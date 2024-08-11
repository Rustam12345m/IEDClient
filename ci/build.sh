#!/bin/bash
# CI script for building IEDClient project

set -e # Exit on error

SCRIPT_PATH="$(dirname "$(realpath "$0")")"
REPO_DIR="$SCRIPT_PATH/../"
BUILD_DIR="$REPO_DIR/build/"
INSTALL_DIR="$REPO_DIR/install/"

build_release()
{
    # Configure the project
    cmake -S "$REPO_DIR" -B "$BUILD_DIR" \
        -DCMAKE_BUILD_TYPE=RelWithDebInfo \
        -DCMAKE_INSTALL_PREFIX="$INSTALL_DIR"/release/ \
        -DBUILD_TESTS=ON

    # Build it
    cmake --build "$BUILD_DIR" --

    # Copy all artifacts
    cmake --install "$BUILD_DIR"
}

build_debug()
{
    # Configure the project
    cmake -S "$REPO_DIR" -B "$BUILD_DIR" \
        -DCMAKE_BUILD_TYPE=Debug \
        -DCMAKE_INSTALL_PREFIX="$INSTALL_DIR"/debug/ \
        -DBUILD_TESTS=ON

    # Build it
    cmake --build "$BUILD_DIR" --

    # Copy all artifacts
    cmake --install "$BUILD_DIR"
}

build_check()
{
    # clang-tidy and sanitizers

    # Configure the project
    cmake -S "$REPO_DIR" -B "$BUILD_DIR" \
        -DCMAKE_BUILD_TYPE=Debug \
        -DCMAKE_INSTALL_PREFIX="$INSTALL_DIR"/check/ \
        -DBUILD_TESTS=ON \
        -DUSE_SANITIZER=undefined,leak \
        -DUSE_LINTER=ON

    # Other code checkers should be placed here: cppcheck

    # Build it
    cmake --build "$BUILD_DIR" --

    # Copy all artifacts
    cmake --install "$BUILD_DIR"
}

simple_rebuild()
{
    # Build it
    cmake --build "$BUILD_DIR" --

    rm -rf "$INSTALL_DIR"/*

    # Copy all artifacts
    cmake --install "$BUILD_DIR"
}

clean_all()
{
    # Clean everything
    rm -rf "$BUILD_DIR"
    rm -rf "$INSTALL_DIR"
    mkdir -p "$BUILD_DIR"
    mkdir -p "$INSTALL_DIR"
}

if [ $# -eq 0 ]; then
    echo "Usage: $0 [ --release | --debug | --check | --clean ]"
fi

# Just for comfortable usage
while [ "$1" != "" ]; do
    case $1 in
        --release )
            clean_all
            build_release
            ;;
        --debug )
            clean_all
            build_debug
            ;;
        --check )
            clean_all
            build_check
            ;;
        --hack )
            simple_rebuild
        --clean )
            clean_all
            ;;
    esac
    shift
done

exit 0