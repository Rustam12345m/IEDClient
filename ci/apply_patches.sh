#!/bin/bash
# Apply all libiec61850 patches. Safe to call multiple times — skips already-applied patches.
# Usage: source ci/apply_patches.sh  (or call directly)

SCRIPT_DIR="$(dirname "$(realpath "$0")")"
REPO_DIR="$(realpath "$SCRIPT_DIR/..")"

cd "$REPO_DIR/3rdparty/libiec61850"
for patch in "$REPO_DIR"/3rdparty/libiec61850-*.patch; do
    [ -f "$patch" ] || continue
    if git apply --check "$patch" 2>/dev/null; then
        echo "==> Applying $(basename "$patch")..."
        git apply "$patch"
    fi
done
cd "$REPO_DIR"
