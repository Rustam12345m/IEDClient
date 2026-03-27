#!/bin/bash
# Inner packaging script for IEDClient — runs inside the Fedora build container.
# Builds the project with CMake and produces a .rpm package in dist/.
#
# Usage:
#   ./ci/rpm/create_package.sh            # Build + create .rpm
#   ./ci/rpm/create_package.sh --no-build # Package only (reuse existing install-rpm/)
#
# For building inside Docker from your host, use ci/rpm/build_local.sh instead.

set -e

SCRIPT_DIR="$(dirname "$(realpath "$0")")"
REPO_DIR="$(realpath "$SCRIPT_DIR/../..")"
BUILD_DIR="$REPO_DIR/build-rpm"
INSTALL_DIR="$REPO_DIR/install-rpm"
DIST_DIR="$REPO_DIR/dist"
NPROC=$(nproc 2>/dev/null || echo 4)

NO_BUILD=false

for arg in "$@"; do
    case "$arg" in
        --no-build) NO_BUILD=true ;;
        --help|-h)
            echo "Usage: $0 [--no-build]"
            echo "  --no-build  Skip the CMake build step (reuse existing install-rpm/)"
            exit 0
            ;;
        *) echo "Unknown option: $arg"; exit 1 ;;
    esac
done

# --- Patches ---
"$SCRIPT_DIR/../apply_patches.sh"

# --- Build ---
if ! $NO_BUILD; then
    echo "==> Configuring (RelWithDebInfo)..."
    rm -rf "$BUILD_DIR" "$INSTALL_DIR"
    cmake -S "$REPO_DIR" -B "$BUILD_DIR" \
        -DCMAKE_BUILD_TYPE=RelWithDebInfo \
        -DCMAKE_INSTALL_PREFIX="$INSTALL_DIR" \
        -DBUILD_TESTS=OFF

    echo "==> Building (j$NPROC)..."
    cmake --build "$BUILD_DIR" -j "$NPROC"

    echo "==> Installing to $INSTALL_DIR..."
    cmake --install "$BUILD_DIR"
fi

BINARY="$INSTALL_DIR/bin/IEDClient"
if [ ! -f "$BINARY" ]; then
    echo "Error: Binary not found at $BINARY"
    echo "       Run a build first or omit --no-build"
    exit 1
fi

# --- Package metadata ---
PKG_NAME="iedclient"
PKG_VERSION="0.0.1"
PKG_RELEASE="1"
PKG_ARCH=$(uname -m)

echo "==> Creating .rpm package ($PKG_NAME $PKG_VERSION-$PKG_RELEASE $PKG_ARCH)..."

RPMBUILD_DIR="$HOME/rpmbuild"
BUILDROOT="$RPMBUILD_DIR/BUILDROOT/${PKG_NAME}-${PKG_VERSION}-${PKG_RELEASE}.${PKG_ARCH}"

mkdir -p "$RPMBUILD_DIR"/{BUILD,RPMS,SOURCES,SPECS,SRPMS}
cp "$SCRIPT_DIR/iedclient.spec" "$RPMBUILD_DIR/SPECS/"

# --- Build the .rpm ---
# Pass build-time paths and version as rpmbuild macros so the spec stays clean.
rpmbuild -bb \
    --define "pkg_version $PKG_VERSION" \
    --define "pkg_release $PKG_RELEASE" \
    --define "install_dir $INSTALL_DIR" \
    --define "script_dir $SCRIPT_DIR" \
    "$RPMBUILD_DIR/SPECS/iedclient.spec"

RPM_FILE=$(find "$RPMBUILD_DIR/RPMS" -name "${PKG_NAME}-${PKG_VERSION}*.rpm" | head -1)

if [ -z "$RPM_FILE" ]; then
    echo "Error: rpmbuild did not produce a .rpm file"
    exit 1
fi

mkdir -p "$DIST_DIR"
cp "$RPM_FILE" "$DIST_DIR/"

DEST="$DIST_DIR/$(basename "$RPM_FILE")"
SIZE=$(du -h "$DEST" | cut -f1)
echo "==> Package created: $DEST ($SIZE)"
echo "    Install with:    sudo dnf install $(basename "$DEST")"
