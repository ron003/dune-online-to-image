#!/bin/bash
# Build script for aarch64 cross-compilation
# Usage: ./build-aarch64.sh

set -e

BUILD_DIR="build-aarch64"
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

echo "Configuring for aarch64 cross-compilation..."
cmake -DCMAKE_TOOLCHAIN_FILE=../.devcontainer/cmake-aarch64-toolchain.cmake \
       -DCMAKE_BUILD_TYPE=Release \
       ..

echo "Building for aarch64..."
cmake --build . --parallel

echo "Build complete! Binaries are in $BUILD_DIR/"
