#!/bin/bash
# Apply all libiec61850 patches. Safe to call multiple times — skips already-applied patches.
# Uses --ignore-whitespace to handle Windows CRLF line endings.

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]:-$0}")" && pwd)"
REPO_DIR="$(cd "$SCRIPT_DIR/.." && pwd)"
SUBMODULE_DIR="$REPO_DIR/3rdparty/libiec61850"

if [ ! -d "$SUBMODULE_DIR/.git" ] && [ ! -f "$SUBMODULE_DIR/.git" ]; then
    echo "Warning: libiec61850 submodule not initialized, skipping patches."
    exit 0
fi

cd "$SUBMODULE_DIR"

applied=0
for patch in "$REPO_DIR"/3rdparty/libiec61850-*.patch; do
    [ -f "$patch" ] || continue
    name="$(basename "$patch")"

    if git apply --ignore-whitespace --check "$patch" 2>/dev/null; then
        echo "==> Applying $name..."
        git apply --ignore-whitespace "$patch"
        applied=$((applied + 1))
    else
        echo "==> Skipping $name (already applied or conflict)"
    fi
done

if [ $applied -gt 0 ]; then
    echo "==> Applied $applied patch(es)."
else
    echo "==> No patches to apply."
fi
