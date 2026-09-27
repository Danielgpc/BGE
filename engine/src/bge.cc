#include "bge.h"

#include "config.h"
#include "defines.h"
#include "image.h"
#include "logging.h"
#include "mutils.h"
#include "platform.h"
#include "shaders.h"

#include <bgfx/bgfx.h>
#include <glm/glm.hpp>

#include <iostream>
#include <string>

#ifndef BGE_SHADER_DIR
#define BGE_SHADER_DIR "build/shaders"
#endif

BGE::BGE() = default;

BGE::~BGE() = default;

// Demo Square
u16 indices[6] = {3, 2, 0, 2, 1, 0};
mutils::Vertex vertices[4] = {{{-0.5f, 0.5f, 0.0f}, 0xffffffff, {0.0f, 0.0f}},
                              {{0.5f, 0.5f, 0.0f}, 0xffffffff, {1.0f, 0.0f}},
                              {{0.5f, -0.5f, 0.0f}, 0xffffffff, {1.0f, 1.0f}},
                              {{-0.5f, -0.5f, 0.0f}, 0xffffffff, {0.0f, 1.0f}}};

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
  case bgfx::RendererType::Nvn:
    return "pssl";
  default:
    return "spirv";
  }
}

int BGE::init() {
  bge::log::init();

  if (!platformInit()) {
    LogError("Failed to initialize platform");
    return -1;
  }

  window = (GLFWwindow *)platformCreateWindow(W_WIDTH, W_HEIGHT, "BGE");
  if (!window) {
    LogError("Failed to create window");
    platformShutdown();
    return -1;
  }

  if (initBGFX(window) != 0) {
    platformDestroyWindow((PlatformWindowHandle)window);
    platformShutdown();
    return -1;
  }

  LogInfo("BGE initialized");
  return 0;
}

int BGE::run() {
  if (!shaderProgram) {
    LogError("No shader program");
    return -1;
  }

  bgfx::IndexBufferHandle ibh =
      bgfx::createIndexBuffer(bgfx::makeRef(indices, sizeof(indices)));
  if (!bgfx::isValid(ibh)) {
    LogError("Failed to create index buffer");
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
    LogError("Failed to create vertex buffer");
    return -1;
  }

  Image *image = new Image("../assets/texture.jpg");
  bgfx::UniformHandle uniform =
      bgfx::createUniform("textureColor", bgfx::UniformType::Sampler);

  bool running = true;
  double lastFrame = 0.0, lastTime = 0.0;
  int frame = 0;
  while (running) {
    if (platformWindowShouldClose((PlatformWindowHandle)window))
      running = false;

    platformPollEvents();

    double now = platformGetTime();
    double deltaTime = now - lastFrame;
    frame++;
    if (now - lastTime >= 1.0) {
      double fps = frame / (now - lastTime);
      std::string title = "Window - FPS: " + std::to_string((int)fps);
      platformSetWindowTitle((PlatformWindowHandle)window, title.c_str());
      frame = 0;
      lastTime = now;
    }

    bgfx::setViewRect(0, 0, 0, W_WIDTH, W_HEIGHT);
    bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x43ff64ff, 1.0f,
                       0);
    if (bgfx::isValid(image->getTextureHandle()))
      bgfx::setTexture(0, uniform, image->getTextureHandle());
    bgfx::setVertexBuffer(0, vbh);
    bgfx::setIndexBuffer(ibh);
    bgfx::submit(0, shaderProgram->getProgramHandle());
    bgfx::frame();
  }

  delete image;
  bgfx::destroy(uniform);
  bgfx::destroy(ibh);
  bgfx::destroy(vbh);
  return 0;
}

int BGE::initBGFX(GLFWwindow *window) {
  bgfx::Init init;
  init.resolution.width = W_WIDTH;
  init.resolution.height = W_HEIGHT;
  init.resolution.reset = BGFX_RESET_VSYNC;
  init.platformData.nwh = platformGetNativeWindowHandle((PlatformWindowHandle)window);
  if (!bgfx::init(init)) {
    LogError("Failed to initialize bgfx");
    return -1;
  }
  bgfxInitialized = true;

  const std::string shaderDir =
      std::string(BGE_SHADER_DIR) + "/" + shaderAPIDir(bgfx::getRendererType());
  LogInfo("bgfx renderer: {} (loading shaders from {})",
          bgfx::getRendererName(bgfx::getRendererType()), shaderDir);

  shaderProgram = new ShaderProgram((shaderDir + "/vs_main.bin").c_str(),
                                    (shaderDir + "/fs_main.bin").c_str());
  if (!bgfx::isValid(shaderProgram->getProgramHandle())) {
    LogError("Failed to create program");
    delete shaderProgram;
    shaderProgram = nullptr;
    return -1;
  }

  return 0;
}

int BGE::shutdown() {
  delete shaderProgram;
  shaderProgram = nullptr;

  if (bgfxInitialized) {
    bgfx::shutdown();
    bgfxInitialized = false;
  }

  if (window) {
    platformDestroyWindow((PlatformWindowHandle)window);
    window = nullptr;
  }

  platformShutdown();
  LogInfo("BGE shutdown");
  bge::log::shutdown();
  return 0;
}