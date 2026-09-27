#!/bin/bash
# Setup script for CMake + vcpkg build

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
VCPKG_ROOT="${VCPKG_ROOT:-$HOME/vcpkg}"

echo "=== BGE CMake + vcpkg Setup ==="

# Check for vcpkg
if [ ! -d "$VCPKG_ROOT" ]; then
    echo "vcpkg not found at $VCPKG_ROOT"
    echo "Installing vcpkg..."
    git clone https://github.com/microsoft/vcpkg.git "$VCPKG_ROOT"
    "$VCPKG_ROOT/bootstrap-vcpkg.sh"
fi

export VCPKG_ROOT="$VCPKG_ROOT"

# Install dependencies via vcpkg manifest
echo "Installing dependencies via vcpkg..."
"$VCPKG_ROOT/vcpkg" install --x-manifest-root="$SCRIPT_DIR"

# Configure with CMake presets
echo "Configuring CMake..."
cmake --preset=default

echo ""
echo "=== Setup Complete ==="
echo ""
echo "Build commands:"
echo "  cmake --build --preset=default        # Build all (Release)"
echo "  cmake --build --preset=debug          # Build all (Debug + sanitizers)"
echo "  cmake --build --preset=release        # Build all (Release)"
echo ""
echo "Run game:"
echo "  ./build/bin/bge_game"
echo ""
echo "Or use Ninja directly:"
echo "  ninja -C build"
echo "  ./build/bin/bge_game"