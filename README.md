# BGE

A small game engine built on [bgfx](https://bkaradzic.github.io/bgfx/), GLFW, Dear ImGui, glm, flecs, and Jolt Physics.

## Requirements

- CMake 3.20+
- Ninja (recommended) or Make
- vcpkg (for dependency management)
- C++20 compiler (GCC 11+, Clang 13+, MSVC 19.30+)

## Setup

### Option 1: Automated setup (recommended)

```sh
./setup_cmake.sh
```

This will:
1. Install vcpkg (if not found)
2. Install all dependencies via vcpkg manifest
3. Configure CMake with Ninja generator

### Option 2: Manual setup

```sh
# Install vcpkg
git clone https://github.com/microsoft/vcpkg.git ~/vcpkg
~/vcpkg/bootstrap-vcpkg.sh

# Install dependencies
export VCPKG_ROOT=~/vcpkg
$VCPKG_ROOT/vcpkg install --x-manifest-root=.

# Configure
cmake --preset=default
```

## Build

```sh
# Build all (Release)
cmake --build --preset=default

# Build Debug (with ASan + UBSan)
cmake --build --preset=debug

# Build Release
cmake --build --preset=release

# Or use Ninja directly
ninja -C build
```

## Run

```sh
# Release build
./build/bin/bge_game

# Debug build
./build/bin/bge_game
```

## Project Layout

```
engine/          Engine library (bge_engine)
  src/           Source files
game/            Game executable (bge_game)
  src/           Entry point
shaders/         Shader sources (.sc) + CMake compilation
third_party/     Legacy Makefile-based deps (deprecated)
build/           All build output (gitignored)
  bin/           Executables & shared libs
  lib/           Static libraries
  shaders/       Compiled shader binaries per API
```

## Shaders

Shader sources live in `shaders/`:
- `vs_*.sc` – vertex shaders
- `fs_*.sc` – fragment shaders
- `varying.def.sc` – attribute/varying definitions

CMake compiles them with bgfx's `shaderc` into `build/shaders/`:
```
build/shaders/
├── metal/    # Metal (macOS)
├── spirv/    # SPIR-V (Vulkan)
├── glsl/     # Desktop OpenGL
├── essl/     # OpenGL ES
├── dxbc/     # Direct3D 11 (Windows)
└── dxil/     # Direct3D 12 (Windows)
```

Only the API bgfx selects at runtime is used; the others are there so the same build works across APIs.

To add a shader: create `shaders/vs_name.sc` and `shaders/fs_name.sc`, they'll be auto-detected and compiled.

## Development

### Presets

| Preset | Build Type | Sanitizers |
|--------|------------|------------|
| `default` | Release | No |
| `debug` | Debug | ASan + UBSan |
| `release` | Release | No |
| `relwithdebinfo` | RelWithDebInfo | No |

### Adding Dependencies

Edit `vcpkg.json` and re-run:
```sh
$VCPKG_ROOT/vcpkg install --x-manifest-root=.
cmake --preset=default
```

### Shader Compilation

Shaders are compiled as part of the build. To force recompile:
```sh
cmake --build --preset=default --target shaders --clean-first
```

## CI

GitHub Actions workflow (`.github/workflows/ci.yml`) tests:
- Linux (Ubuntu), macOS, Windows
- Debug + Release builds
- Clang-tidy static analysis
- clang-format style check

## Legacy Makefile

The original Makefile-based build is still available:
```sh
make
make run
```
But it's deprecated — use CMake for new development.

## License

MIT