#!/bin/bash
# Build IEDClient inside a Docker/Podman container and store artifacts locally.
#
# Usage:
#   ./ci/build_local.sh                     # RelWithDebInfo build + package
#   ./ci/build_local.sh --debug             # Debug build + package
#   ./ci/build_local.sh --check             # Debug + sanitizers + linter
#   ./ci/build_local.sh --release           # RelWithDebInfo build only (no package)
#   ./ci/build_local.sh --release --archive # Explicit build + package
#   ./ci/build_local.sh --rebuild-image     # Force rebuild the container image
#
# Environment:
#   DOCKER=podman ./ci/build_local.sh       # Use podman instead of docker

set -e

SCRIPT_DIR="$(dirname "$(realpath "$0")")"
REPO_DIR="$(realpath "$SCRIPT_DIR/..")"

DOCKER="${DOCKER:-podman}"
IMAGE_NAME="iedclient:linux"
CONTAINER_WORKDIR="/workdir"

REBUILD_IMAGE=false
BUILD_ARGS=()

usage() {
    cat <<EOF
Usage: $0 [options]

Builds IEDClient inside a container and copies artifacts to the host.

Build options (passed to ci/build_inner.sh inside the container):
  --release         RelWithDebInfo build (default if no build type given)
  --debug           Debug build
  --check           Debug + clang-tidy + sanitizers
  --archive         Create self-contained AppImage (default when no flags given)

Local options:
  --rebuild-image   Force rebuild the container image before building
  --help            Show this help

Environment variables:
  DOCKER            Container runtime to use (default: podman)
                    Example: DOCKER=docker $0 --release

When called with no arguments, runs: --release --archive

Artifacts are stored in:
  install/          Installed build output (binary, headers)
  dist/             AppImage (self-contained portable binary)
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
        "$DOCKER" build -f "$SCRIPT_DIR/Dockerfile.Linux" -t "$IMAGE_NAME" "$SCRIPT_DIR/"
    else
        echo "==> Container image $IMAGE_NAME already exists (use --rebuild-image to force)"
    fi
}

run_build() {
    echo "==> Running build inside container ($DOCKER)..."
    echo "    Arguments: ${BUILD_ARGS[*]}"

    local run_opts=(--rm -v "$REPO_DIR":"$CONTAINER_WORKDIR" -w "$CONTAINER_WORKDIR")

    # Podman rootless: map host UID/GID into the container so the
    # builder user can read/write the mounted volume.
    if [ "$(basename "$DOCKER")" = "podman" ]; then
        run_opts+=(--userns=keep-id)
    fi

    "$DOCKER" run "${run_opts[@]}" \
        "$IMAGE_NAME" \
        "$CONTAINER_WORKDIR/ci/build_inner.sh" "${BUILD_ARGS[@]}"
}

# --- Parse arguments ---
while [ "$1" != "" ]; do
    case "$1" in
        --rebuild-image)
            REBUILD_IMAGE=true
            ;;
        --help|-h)
            usage
            ;;
        *)
            BUILD_ARGS+=("$1")
            ;;
    esac
    shift
done

# Default: --release --archive when no build arguments given
if [ ${#BUILD_ARGS[@]} -eq 0 ]; then
    BUILD_ARGS=(--release --archive)
fi

init_submodules
build_image
run_build

# --- Report results ---
echo ""
if [ -d "$REPO_DIR/dist" ]; then
    echo "==> Artifacts in dist/:"
    ls -lh "$REPO_DIR"/dist/*.AppImage 2>/dev/null || ls -lh "$REPO_DIR/dist/"
fi
echo "==> Done."
