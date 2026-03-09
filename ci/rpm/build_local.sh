#!/bin/bash
# Build IEDClient and package it as a .rpm inside a Docker/Podman container.
#
# Usage:
#   ./ci/rpm/build_local.sh                  # Build + create .rpm
#   ./ci/rpm/build_local.sh --no-build       # Re-package only (reuse install/)
#   ./ci/rpm/build_local.sh --rebuild-image  # Force rebuild the container image
#
# Environment:
#   DOCKER=podman ./ci/rpm/build_local.sh    # Use podman instead of docker

set -e

SCRIPT_DIR="$(dirname "$(realpath "$0")")"
REPO_DIR="$(realpath "$SCRIPT_DIR/../..")"

DOCKER="${DOCKER:-docker}"
IMAGE_NAME="iedclient:rpm"
CONTAINER_WORKDIR="/workdir"

REBUILD_IMAGE=false
PACKAGE_ARGS=()

usage() {
    cat <<EOF
Usage: $0 [options]

Builds IEDClient inside a Fedora container and produces a .rpm package.

Options:
  --no-build        Skip the CMake build step (reuse existing install/)
  --rebuild-image   Force rebuild the container image before building
  --help            Show this help

Environment variables:
  DOCKER            Container runtime to use (default: docker)
                    Example: DOCKER=podman $0

Artifact is stored in:
  dist/             The generated .rpm package
EOF
    exit 0
}

init_submodules() {
    if [ ! -f "$REPO_DIR/3rdparty/libiec61850/CMakeLists.txt" ] || \
       [ ! -f "$REPO_DIR/3rdparty/googletest/CMakeLists.txt" ]; then
        echo "==> Initializing git submodules on host..."
        git -C "$REPO_DIR" submodule update --init --recursive
    fi
}

build_image() {
    local image_exists
    image_exists=$("$DOCKER" images -q "$IMAGE_NAME" 2>/dev/null)

    if [ -z "$image_exists" ] || $REBUILD_IMAGE; then
        echo "==> Building container image ($IMAGE_NAME) using $DOCKER..."
        "$DOCKER" build -f "$SCRIPT_DIR/../Dockerfile.Fedora" -t "$IMAGE_NAME" "$SCRIPT_DIR/../"
    else
        echo "==> Container image $IMAGE_NAME already exists (use --rebuild-image to force)"
    fi
}

run_package() {
    echo "==> Running packaging inside container ($DOCKER)..."

    local run_opts=(--rm -v "$REPO_DIR":"$CONTAINER_WORKDIR" -w "$CONTAINER_WORKDIR")

    "$DOCKER" run "${run_opts[@]}" \
        "$IMAGE_NAME" \
        "$CONTAINER_WORKDIR/ci/rpm/create_package.sh" "${PACKAGE_ARGS[@]}"
}

# --- Parse arguments ---
while [ "$1" != "" ]; do
    case "$1" in
        --rebuild-image) REBUILD_IMAGE=true ;;
        --help|-h)       usage ;;
        *)               PACKAGE_ARGS+=("$1") ;;
    esac
    shift
done

init_submodules
build_image
run_package

# --- Report results ---
echo ""
if [ -d "$REPO_DIR/dist" ]; then
    echo "==> Artifacts in dist/:"
    ls -lh "$REPO_DIR"/dist/*.rpm 2>/dev/null || ls -lh "$REPO_DIR/dist/"
fi
echo "==> Done."
