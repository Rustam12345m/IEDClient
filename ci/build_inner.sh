#!/bin/bash
# Inner build script for IEDClient — runs inside the build environment
# (either a Docker container or a host with all dependencies installed).
#
# Usage:
#   ./ci/build_inner.sh --release             # RelWithDebInfo build
#   ./ci/build_inner.sh --debug               # Debug build
#   ./ci/build_inner.sh --check               # Debug + sanitizers + linter
#   ./ci/build_inner.sh --hack                # Incremental rebuild (no clean)
#   ./ci/build_inner.sh --release --archive   # Build and create distributable archive
#   ./ci/build_inner.sh --archive             # Package an existing build
#   ./ci/build_inner.sh --clean               # Remove all build artifacts
#
# For building inside Docker from your host, use ci/build_local.sh instead.

set -e

SCRIPT_DIR="$(dirname "$(realpath "$0")")"
REPO_DIR="$(realpath "$SCRIPT_DIR/..")"
BUILD_DIR="$REPO_DIR/build"
INSTALL_DIR="$REPO_DIR/install"
DIST_DIR="$REPO_DIR/dist"
NPROC=$(nproc 2>/dev/null || echo 4)

BUILD_TYPE=""
DO_ARCHIVE=false
DO_CLEAN=false
DO_HACK=false

usage() {
    cat <<EOF
Usage: $0 [options]

Build options:
  --release     RelWithDebInfo build
  --debug       Debug build
  --check       Debug + clang-tidy + sanitizers (undefined, leak)
  --hack        Incremental rebuild only (no clean)

Other options:
  --archive     Create distributable archive with binary, libs, and Qt plugins
  --clean       Remove build/, install/, and dist/ directories
  --help        Show this help

Options can be combined:  $0 --release --archive
EOF
    exit 0
}

clean_all() {
    echo "==> Cleaning build artifacts..."
    rm -rf "$BUILD_DIR" "$INSTALL_DIR" "$DIST_DIR"
}

do_build() {
    local cmake_build_type="$1"
    shift
    local extra_args=("$@")

    mkdir -p "$BUILD_DIR"

    echo "==> Configuring ($cmake_build_type)..."
    cmake -S "$REPO_DIR" -B "$BUILD_DIR" \
        -DCMAKE_BUILD_TYPE="$cmake_build_type" \
        -DCMAKE_INSTALL_PREFIX="$INSTALL_DIR" \
        -DBUILD_TESTS=ON \
        "${extra_args[@]}"

    echo "==> Building (j$NPROC)..."
    cmake --build "$BUILD_DIR" -j "$NPROC"

    echo "==> Installing to $INSTALL_DIR..."
    cmake --install "$BUILD_DIR"
}

# Resolve the Qt paths for plugin/QML collection
qt_query() {
    local key="$1"
    qmake6 -query "$key" 2>/dev/null \
        || qtpaths6 --"$(echo "$key" | sed 's/QT_INSTALL_//' | tr '[:upper:]' '[:lower:]')"-dir 2>/dev/null \
        || echo ""
}

do_archive() {
    local binary="$INSTALL_DIR/bin/IEDClient"
    if [ ! -f "$binary" ]; then
        echo "Error: Binary not found at $binary"
        echo "       Run a build first:  $0 --release"
        exit 1
    fi

    echo "==> Packaging distributable archive..."

    local pkg="$DIST_DIR/IEDClient"
    rm -rf "$DIST_DIR"
    mkdir -p "$pkg/lib" "$pkg/plugins/platforms" "$pkg/qml"

    # --- Binary ---
    cp "$binary" "$pkg/"

    # Libs that are safe to assume exist on any modern Linux desktop — skip bundling them.
    local skip_libs="linux-vdso|ld-linux|libc\.so|libm\.so|libdl\.so|librt\.so|libpthread"
    skip_libs+="|libgcc_s|libstdc\+\+"
    skip_libs+="|libX11\.so|libXext|libXrender|libxcb\.so|libxcb-|libxkb"
    skip_libs+="|libdrm|libgbm|libvulkan"
    skip_libs+="|libwayland-client|libwayland-server|libwayland-egl"
    skip_libs+="|libfontconfig|libfreetype|libharfbuzz|libexpat|libz\.so|libpng"
    skip_libs+="|libglib-2|libgobject|libgio-2|libgmodule|libdbus-1|libsystemd|libcap"
    skip_libs+="|libgpg|liblzma|libzstd|liblz4|libgcrypt|libcom_err"
    skip_libs+="|libkrb5|libk5crypto|libkeyutils|libbrotli|libpcre|libselinux"
    skip_libs+="|libmount|libblkid|libffi|libresolv|libnss|libnsl|libmd|libbsd"

    # --- Qt libraries (explicit — ldd misses runtime-loaded Qt modules) ---
    local qt_lib_path
    qt_lib_path="$(qt_query QT_INSTALL_LIBS)"
    if [ -n "$qt_lib_path" ] && [ -d "$qt_lib_path" ]; then
        echo "  Collecting Qt libraries from $qt_lib_path..."
        find "$qt_lib_path" -maxdepth 1 -name 'libQt6*.so.*' | while read -r lib; do
            echo "    $(basename "$lib")"
            cp -L "$lib" "$pkg/lib/"
        done
    fi

    # --- Qt platform plugins ---
    local qt_plugin_path
    qt_plugin_path="$(qt_query QT_INSTALL_PLUGINS)"

    if [ -n "$qt_plugin_path" ] && [ -d "$qt_plugin_path" ]; then
        echo "  Collecting Qt plugins from $qt_plugin_path..."

        # Platform plugins
        if [ -d "$qt_plugin_path/platforms" ]; then
            cp -L "$qt_plugin_path/platforms"/libqxcb.so      "$pkg/plugins/platforms/" 2>/dev/null || true
            cp -L "$qt_plugin_path/platforms"/libqwayland*.so  "$pkg/plugins/platforms/" 2>/dev/null || true
        fi

        # Other plugin categories
        for pdir in xcbglintegrations egldeviceintegrations platformthemes imageformats iconengines; do
            if [ -d "$qt_plugin_path/$pdir" ]; then
                mkdir -p "$pkg/plugins/$pdir"
                cp -rL "$qt_plugin_path/$pdir"/*.so "$pkg/plugins/$pdir/" 2>/dev/null || true
            fi
        done
    fi

    # --- Non-Qt shared library deps of the binary ---
    echo "  Collecting shared libraries..."
    ldd "$binary" | grep "=> /" | awk '{print $3}' | sort -u | while read -r lib; do
        local base
        base="$(basename "$lib")"
        if [ ! -f "$pkg/lib/$base" ] && ! echo "$lib" | grep -qE "$skip_libs"; then
            echo "    $base"
            cp -L "$lib" "$pkg/lib/"
        fi
    done

    # --- Transitive deps of all bundled .so files (plugins + qml + libs) ---
    echo "  Collecting transitive dependencies..."
    find "$pkg" \( -name '*.so' -o -name '*.so.*' \) | while read -r f; do
        ldd "$f" 2>/dev/null | grep "=> /" | awk '{print $3}'
    done | sort -u | while read -r lib; do
        local base
        base="$(basename "$lib")"
        if [ ! -f "$pkg/lib/$base" ] && ! echo "$lib" | grep -qE "$skip_libs"; then
            echo "    $base"
            cp -L "$lib" "$pkg/lib/"
        fi
    done

    # --- QML modules ---
    local qt_qml_path
    qt_qml_path="$(qt_query QT_INSTALL_QML)"

    if [ -n "$qt_qml_path" ] && [ -d "$qt_qml_path" ]; then
        echo "  Collecting QML modules from $qt_qml_path..."
        cp -rL "$qt_qml_path"/. "$pkg/qml/"
    fi

    # --- Launcher script ---
    cat > "$pkg/IEDClient.sh" <<'LAUNCHER'
#!/bin/bash
DIR="$(dirname "$(realpath "$0")")"
export LD_LIBRARY_PATH="$DIR/lib${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"
export QT_PLUGIN_PATH="$DIR/plugins${QT_PLUGIN_PATH:+:$QT_PLUGIN_PATH}"
export QML2_IMPORT_PATH="$DIR/qml${QML2_IMPORT_PATH:+:$QML2_IMPORT_PATH}"
exec "$DIR/IEDClient" "$@"
LAUNCHER
    chmod +x "$pkg/IEDClient.sh"

    # --- Archive ---
    local arch
    arch="$(uname -m)"
    local archive="$DIST_DIR/IEDClient-linux-${arch}.tar.gz"

    echo "  Creating archive..."
    tar -czf "$archive" -C "$DIST_DIR" IEDClient/

    local size
    size="$(du -h "$archive" | cut -f1)"
    echo "==> Package created: $archive ($size)"
    echo "    Extract and run:  tar xzf $(basename "$archive") && ./IEDClient/IEDClient.sh"
}

# --- Parse arguments ---
if [ $# -eq 0 ]; then
    usage
fi

while [ "$1" != "" ]; do
    case "$1" in
        --release)  BUILD_TYPE="release" ;;
        --debug)    BUILD_TYPE="debug" ;;
        --check)    BUILD_TYPE="check" ;;
        --hack)     DO_HACK=true ;;
        --archive)  DO_ARCHIVE=true ;;
        --clean)    DO_CLEAN=true ;;
        --help|-h)  usage ;;
        *)          echo "Unknown option: $1"; usage ;;
    esac
    shift
done

# --- Execute ---
if $DO_CLEAN; then
    clean_all
    exit 0
fi

if $DO_HACK; then
    echo "==> Incremental rebuild..."
    cmake --build "$BUILD_DIR" -j "$NPROC"
    cmake --install "$BUILD_DIR"
elif [ -n "$BUILD_TYPE" ]; then
    case "$BUILD_TYPE" in
        release)
            clean_all
            do_build RelWithDebInfo
            ;;
        debug)
            clean_all
            do_build Debug
            ;;
        check)
            clean_all
            do_build Debug -DUSE_SANITIZER=undefined,leak -DUSE_LINTER=ON
            ;;
    esac
fi

if $DO_ARCHIVE; then
    do_archive
fi

echo "==> Done."
