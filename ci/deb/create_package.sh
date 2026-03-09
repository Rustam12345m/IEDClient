#!/bin/bash
# Inner packaging script for IEDClient — runs inside the build container.
# Builds the project with CMake and produces a .deb package in dist/.
#
# Usage:
#   ./ci/deb/create_package.sh            # Build + create .deb
#   ./ci/deb/create_package.sh --no-build # Package only (reuse existing install-deb/)
#
# For building inside Docker from your host, use ci/deb/build_local.sh instead.

set -e

SCRIPT_DIR="$(dirname "$(realpath "$0")")"
REPO_DIR="$(realpath "$SCRIPT_DIR/../..")"
BUILD_DIR="$REPO_DIR/build-deb"
INSTALL_DIR="$REPO_DIR/install-deb"
DIST_DIR="$REPO_DIR/dist"
NPROC=$(nproc 2>/dev/null || echo 4)

NO_BUILD=false

for arg in "$@"; do
    case "$arg" in
        --no-build) NO_BUILD=true ;;
        --help|-h)
            echo "Usage: $0 [--no-build]"
            echo "  --no-build  Skip the CMake build step (reuse existing install-deb/)"
            exit 0
            ;;
        *) echo "Unknown option: $arg"; exit 1 ;;
    esac
done

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
PKG_ARCH=$(dpkg --print-architecture)
PKG_DIR="$DIST_DIR/${PKG_NAME}_${PKG_VERSION}_${PKG_ARCH}"

echo "==> Creating .deb package ($PKG_NAME $PKG_VERSION $PKG_ARCH)..."

rm -rf "$PKG_DIR" "${PKG_DIR}.deb"
mkdir -p "$PKG_DIR/usr/bin"
mkdir -p "$PKG_DIR/usr/share/applications"
mkdir -p "$PKG_DIR/DEBIAN"

# --- Binary ---
cp "$BINARY" "$PKG_DIR/usr/bin/IEDClient"
chmod 755 "$PKG_DIR/usr/bin/IEDClient"

# --- Desktop entry ---
cp "$SCRIPT_DIR/iedclient.desktop" "$PKG_DIR/usr/share/applications/iedclient.desktop"

# --- DEBIAN/control (substitute version and arch placeholders) ---
sed -e "s/@VERSION@/$PKG_VERSION/" \
    -e "s/@ARCH@/$PKG_ARCH/" \
    "$SCRIPT_DIR/control" > "$PKG_DIR/DEBIAN/control"

# --- Build the .deb ---
fakeroot dpkg-deb --root-owner-group --build "$PKG_DIR"

DEB_FILE="${PKG_DIR}.deb"
SIZE=$(du -h "$DEB_FILE" | cut -f1)
echo "==> Package created: $DEB_FILE ($SIZE)"
echo "    Install with:    sudo dpkg -i $(basename "$DEB_FILE")"
