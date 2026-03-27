#!/bin/bash
# Apply all libiec61850 patches. Safe to call multiple times — skips already-applied patches.
# Handles Windows CRLF by re-checking out with LF if needed.

set -e

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
REPO_DIR="$(cd "$SCRIPT_DIR/.." && pwd)"

cd "$REPO_DIR/3rdparty/libiec61850"

# On Windows (MINGW/MSYS), git may check out with CRLF — patches expect LF.
case "$(uname -s)" in
    MINGW*|MSYS*|CYGWIN*)
        git config core.autocrlf false
        git rm -r --cached . -q 2>/dev/null || true
        git checkout -- .
        ;;
esac

for patch in "$REPO_DIR"/3rdparty/libiec61850-*.patch; do
    [ -f "$patch" ] || continue
    if git apply --check "$patch" 2>/dev/null; then
        echo "==> Applying $(basename "$patch")..."
        git apply "$patch"
    fi
done

cd "$REPO_DIR"
