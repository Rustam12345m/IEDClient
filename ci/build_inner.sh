#!/bin/bash
# Inner build script for IEDClient — runs inside the build environment
# (either a Docker container or a host with all dependencies installed).
#
# Usage:
#   ./ci/build_inner.sh --release             # RelWithDebInfo build
#   ./ci/build_inner.sh --debug               # Debug build
#   ./ci/build_inner.sh --check               # Debug + sanitizers + linter
#   ./ci/build_inner.sh --hack                # Incremental rebuild (no clean)
#   ./ci/build_inner.sh --release --archive   # Build and create AppImage
#   ./ci/build_inner.sh --archive             # Package an existing build into AppImage
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
  --archive     Create self-contained AppImage (binary + libs + Qt plugins)
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
    cmake --install "$BUILD_DIR" > /dev/null
}

do_appimage() {
    local binary="$INSTALL_DIR/bin/IEDClient"
    if [ ! -f "$binary" ]; then
        echo "Error: Binary not found at $binary"
        echo "       Run a build first:  $0 --release"
        exit 1
    fi

    echo "==> Packaging AppImage..."

    local appdir="$BUILD_DIR/IEDClient.AppDir"
    rm -rf "$appdir" "$DIST_DIR"
    mkdir -p "$DIST_DIR"

    # --- Icon ---
    # Use resources/IEDClient.png if present; otherwise generate a placeholder
    # from an existing SVG icon.  Replace resources/IEDClient.png with a proper
    # 256x256 app icon to use it automatically.
    local icon="$REPO_DIR/resources/IEDClient.png"
    if [ ! -f "$icon" ]; then
        echo "  resources/IEDClient.png not found — generating placeholder icon..."
        if ! command -v rsvg-convert &>/dev/null; then
            echo "Error: rsvg-convert not found (librsvg2-bin)."
            echo "       Either add resources/IEDClient.png or rebuild the Docker image:"
            echo "         ./ci/build_local.sh --rebuild-image"
            exit 1
        fi
        icon="$BUILD_DIR/IEDClient.png"
        rsvg-convert -w 256 -h 256 "$REPO_DIR/ui/icons/hub.svg" -o "$icon"
    fi

    # --- linuxdeploy with Qt plugin bundles everything automatically ---
    # QMAKE    — tells the Qt plugin where Qt is installed.
    # QMLDIR   — source QML tree; the plugin scans it for import statements to
    #             decide which QML modules to bundle.
    # OUTPUT   — destination path for the finished AppImage file.
    export QMAKE=qmake6
    export QML_SOURCES_PATHS="$REPO_DIR/ui"
    export OUTPUT="$DIST_DIR/IEDClient-linux-$(uname -m).AppImage"

    linuxdeploy \
        --appdir "$appdir" \
        -e "$binary" \
        -d "$REPO_DIR/resources/IEDClient.desktop" \
        -i "$icon" \
        --plugin qt \
        --output appimage \
        > /dev/null

    local size
    size="$(du -h "$OUTPUT" | cut -f1)"
    echo "==> AppImage created: $OUTPUT ($size)"
    echo "    Run with:  chmod +x $(basename "$OUTPUT") && ./$(basename "$OUTPUT")"
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
    cmake --install "$BUILD_DIR" > /dev/null
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
    do_appimage
fi

echo "==> Done."
