#include "bge.h"

#include "config.h"
#include "defines.h"
#include "mutils.h"
#include "shaders.h"

#include <GLFW/glfw3.h>
#include <bgfx/bgfx.h>
#include <glm/glm.hpp>

#ifdef _WIN32
#define GLFW_EXPOSE_NATIVE_WIN32
#elif __APPLE__
#define GLFW_EXPOSE_NATIVE_COCOA
#elif __linux__
#define GLFW_EXPOSE_NATIVE_X11
// Or if you use Wayland: #define GLFW_EXPOSE_NATIVE_WAYLAND
#endif

#include <GLFW/glfw3native.h>

#include <iostream>
#include <string>

#ifndef BGE_SHADER_DIR
#define BGE_SHADER_DIR "build/shaders"
#endif

BGE::BGE() = default;

BGE::~BGE() = default;

// Demo Square
u16 indices[6] = {3, 2, 0, 2, 1, 0};
mutils::Vertex vertices[4] = {{{-0.5f, 0.5f, 0.0f}, 0xff0000ff, {0.0f, 0.0f}},
                              {{0.5f, 0.5f, 0.0f}, 0x00ff00ff, {1.0f, 0.0f}},
                              {{0.5f, -0.5f, 0.0f}, 0x0000ffff, {1.0f, 1.0f}},
                              {{-0.5f, -0.5f, 0.0f}, 0xff00ffff, {0.0f, 1.0f}}};

// Maps the renderer bgfx selected at init to the shader binary directory
// produced by shaders/Makefile (build/shaders/<api>/).
static const char *shaderAPIDir(bgfx::RendererType::Enum type) {
  switch (type) {
  case bgfx::RendererType::Metal:
    return "metal";
  case bgfx::RendererType::Vulkan:
    return "spirv";
  case bgfx::RendererType::OpenGL:
    return "glsl";
  case bgfx::RendererType::OpenGLES:
    return "essl";
  case bgfx::RendererType::Direct3D11:
    return "dxbc";
  case bgfx::RendererType::Direct3D12:
    return "dxil";
  case bgfx::RendererType::Agc:
  case bgfx::RendererType::Gnm:
    return "pssl";
  case bgfx::RendererType::WebGPU:
    return "wgsl";
  default:
    return "spirv";
  }
}

int BGE::init() {
  window = initWindow();
  if (!window) {
    std::cerr << "Failed to create GLFW window" << std::endl;
    return -1;
  }
  initBGFX(window);
  return 0;
}

int BGE::run() {
  if (!shaderProgram) {
    std::cerr << "No shader program" << std::endl;
    return -1;
  }
  bgfx::IndexBufferHandle ibh =
      bgfx::createIndexBuffer(bgfx::makeRef(indices, sizeof(indices)));
  if (!bgfx::isValid(ibh)) {
    std::cerr << "Failed to create index buffer" << std::endl;
    return -1;
  }
  bgfx::VertexLayout vertexLayout;
  vertexLayout.begin()
      .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
      .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
      .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
      .end();

  bgfx::VertexBufferHandle vbh = bgfx::createVertexBuffer(
      bgfx::makeRef(vertices, sizeof(vertices)), vertexLayout);
  if (!bgfx::isValid(vbh)) {
    std::cerr << "Failed to create vertex buffer" << std::endl;
    return -1;
  }

  bool running = true;
  while (running) {
    if (glfwWindowShouldClose(window)) {
      running = false;
    }
    glfwPollEvents();
    //> Render
    bgfx::setViewRect(0, 0, 0, W_WIDTH, W_HEIGHT);
    bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x43ff64ff, 1.0f,
                       0);
    bgfx::setVertexBuffer(0, vbh);
    bgfx::setIndexBuffer(ibh);
    bgfx::setState(BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A |
                   BGFX_STATE_WRITE_Z | BGFX_STATE_DEPTH_TEST_LESS);
    bgfx::submit(0, shaderProgram->getProgramHandle());
    bgfx::frame();
    //< Render
  }
  bgfx::destroy(ibh);
  bgfx::destroy(vbh);
  return 0;
}

GLFWwindow *BGE::initWindow() {
  glfwInit();
  GLFWwindow *window =
      glfwCreateWindow(W_WIDTH, W_HEIGHT, "BGE", nullptr, nullptr);
  return window;
}

inline void *GetNativeWindowHandle(GLFWwindow *glfwWindow) {
#ifdef _WIN32
  // Returns HWND (Windows)
  return (void *)glfwGetWin32Window(glfwWindow);
#elif __APPLE__
  // Returns NSWindow* (macOS)
  return (void *)glfwGetCocoaWindow(glfwWindow);
#elif __linux__
  // Returns Window (X11)
  return (void *)(uintptr_t)glfwGetX11Window(glfwWindow);
#else
  return nullptr;
#endif
}

int BGE::initBGFX(GLFWwindow *window) {
  bgfx::Init init;
  init.swapChain.height = W_HEIGHT;
  init.swapChain.width = W_WIDTH;
  init.swapChain.nwh = GetNativeWindowHandle(window);
  if (!bgfx::init(init)) {
    std::cerr << "Failed to initialize bgfx" << std::endl;
    return -1;
  }

  const std::string shaderDir =
      std::string(BGE_SHADER_DIR) + "/" + shaderAPIDir(bgfx::getRendererType());
  std::cout << "bgfx renderer: "
            << bgfx::getRendererName(bgfx::getRendererType())
            << " (loading shaders from " << shaderDir << ")" << std::endl;

  shaderProgram = new ShaderProgram((shaderDir + "/vs_main.bin").c_str(),
                                    (shaderDir + "/fs_main.bin").c_str());
  if (!bgfx::isValid(shaderProgram->getProgramHandle())) {
    std::cerr << "Failed to create program" << std::endl;
    delete shaderProgram;
    shaderProgram = nullptr;
    return -1;
  }
  return 0;
}

int BGE::shutdown() {
  delete shaderProgram;
  shaderProgram = nullptr;
  bgfx::shutdown();
  glfwDestroyWindow(window);
  glfwTerminate();
  return 0;
}
