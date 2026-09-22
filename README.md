# BGE

A small game engine built on [bgfx](https://bkaradzic.github.io/bgfx/), GLFW, Dear ImGui and glm.

## Requirements

- `git`, `make`, `cmake`
- C++ compiler (Xcode Command Line Tools on macOS, GCC/Clang on Linux)

## Setup

Clone the third-party dependencies into `third_party/`:

```sh
./setup.sh
```

## Build

```sh
make
```

This builds, in order:

1. **third_party** – bgfx, bimg, bx (via genie), GLFW, ImGui, and bgfx's `shaderc` compiler
2. **engine** – the static engine library (`build/engine/libengine.a`)
3. **shaders** – shader sources compiled for every rendering API (see below)
4. **game** – the game executable (`build/game/bgeTestGame`)

## Run

```sh
make run
```

On startup bgfx picks the best renderer for your platform (e.g. Metal on macOS) and the engine loads the matching pre-compiled shader from `build/shaders/<api>/`.

## Shaders

Shader sources live in `shaders/`:

- `vs_*.sc` – vertex shaders
- `fs_*.sc` – fragment shaders
- `varying.def.sc` – attribute/varying definitions

`make` compiles them with bgfx's `shaderc` into `build/shaders/`:

```
build/shaders/
├── essl/    # OpenGL ES
├── glsl/    # desktop OpenGL
├── metal/   # Metal (macOS)
└── spirv/   # Vulkan
```

Only the API bgfx selects at runtime is used; the others are there so the same build works across APIs.

To add a shader: create `shaders/vs_name.sc` and `shaders/fs_name.sc`, then load it in code:

```cpp
ShaderProgram program(".../metal/vs_name.bin", ".../metal/fs_name.bin");
```

(pick the directory that matches the renderer bgfx selected).

## Project layout

```
engine/    engine code (window, bgfx setup, render loop, ShaderProgram)
game/      game executable entry point
shaders/   shader sources + Makefile for shader compilation
third_party/  dependencies (created by setup.sh)
build/     all build output (binaries, libraries, compiled shaders)
```

## Clean

```sh
make clean
```

Removes `build/` output for the engine, game and shaders. Third-party builds are kept (uncomment the line in the root `Makefile` to clean those too).
