#!/bin/bash

set -e  # Exit on error

# Configuration
BUILD_DIR=".cmakeData"
BIN_SUBDIR="build"
EXECUTABLE="opengl-toy"  # Change if your project name differs

echo "🧹 Cleaning previous build..."
rm -rf "$BUILD_DIR"

echo "⚙️ Configuring CMake..."
cmake -S . -B "$BUILD_DIR"

echo "🔨 Building project..."
cmake --build "$BUILD_DIR"

echo "🚀 Running executable..."
"$BUILD_DIR/$BIN_SUBDIR/$EXECUTABLE"
