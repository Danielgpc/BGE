/**
 * @file bge.cc
 * @brief Implementation of the BGE engine: init, main loop and shutdown.
 */

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

/**
 * @def BGE_SHADER_DIR
 * @brief Root directory containing the per-API compiled shader folders.
 *
 * Overridable at compile time (see shaders/Makefile); defaults to
 * "build/shaders" when nothing was passed by the build system.
 */
#ifndef BGE_SHADER_DIR
#define BGE_SHADER_DIR "build/shaders"
#endif

/** @brief Default constructor; members already have their default initializers. */
BGE::BGE() = default;

/**
 * @brief Default destructor.
 *
 * Deliberately empty: resources are released only by shutdown(), which must be
 * called explicitly by the host application.
 */
BGE::~BGE() = default;

/**
 * @brief Index data for the demo quad (two triangles), in vertex-buffer order.
 *
 * Each value is an offset into #vertices; the pattern 3,2,0 / 2,1,0 produces a
 * counter-clockwise winding so the quad faces the camera.
 */
u16 indices[6] = {3, 2, 0, 2, 1, 0};

/**
 * @brief Vertex data for the demo quad: position, packed color and UVs.
 *
 * The quad spans [-0.5, 0.5] on X/Y, is white (0xffffffff) and maps the whole
 * texture onto it. Layout must match the VertexLayout built in BGE::run().
 */
mutils::Vertex vertices[4] = {{{-0.5f, 0.5f, 0.0f}, 0xffffffff, {0.0f, 0.0f}},
                              {{0.5f, 0.5f, 0.0f}, 0xffffffff, {1.0f, 0.0f}},
                              {{0.5f, -0.5f, 0.0f}, 0xffffffff, {1.0f, 1.0f}},
                              {{-0.5f, -0.5f, 0.0f}, 0xffffffff, {0.0f, 1.0f}}};

/**
 * @brief Maps the renderer bgfx selected at init to the shader binary directory
 *        produced by shaders/Makefile (build/shaders/<api>/).
 *
 * The directory name is the cross-compiled backend suffix, so the runtime picks
 * the exact same binary format it will feed to the GPU.
 *
 * @param type Renderer backend chosen by bgfx during initialization.
 * @return Directory name for that backend ("metal", "spirv", "glsl", ...);
 *         unknown backends fall back to "spirv".
 */
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

/**
 * @brief Initializes the platform, the window and bgfx, in that order.
 *
 * Every step undoes the previous ones on failure so the process never leaks a
 * partially initialized subsystem:
 * 1. platformInit() starts the windowing library.
 * 2. The window is created with the size/title from config.h.
 * 3. initBGFX() creates the swap chain and loads the shaders.
 *
 * @return 0 on success, -1 if the platform, the window or bgfx failed.
 */
int BGE::init() {
  bge::log::init();

  if (!platformInit()) {
    LogError("Failed to initialize platform");
    return -1;
  }

  window = (GLFWwindow *)platformCreateWindow(W_WIDTH, W_HEIGHT, W_TITLE);
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

/**
 * @brief Main loop: upload geometry/texture, then render one quad per frame.
 *
 * Setup performed once before looping:
 * - the index buffer from #indices,
 * - the vertex layout (position, color, UV) matching mutils::Vertex,
 * - the vertex buffer from #vertices,
 * - the texture from ../assets/texture.jpg and its sampler uniform.
 *
 * Each iteration polls events, measures the frame delta, updates the FPS text
 * in the title bar once per second, then clears view 0, binds the texture and
 * submits the quad before handing the frame to bgfx. GPU objects are released
 * when the loop exits.
 *
 * @return 0 on success, -1 if there is no shader program or buffer creation
 *         failed.
 */
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
    // Once a second, turn the accumulated frame count into an FPS reading and
    // publish it in the window title, then reset the counters.
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
    // Only bind the texture if it actually loaded; otherwise leave slot 0 stale
    // rather than handing bgfx an invalid handle.
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

/**
 * @brief Creates the bgfx swap chain for @p window and loads the shaders.
 *
 * The native window handle is handed to bgfx so it can render into the GLFW
 * window. Afterwards the renderer type reported by bgfx is used to locate the
 * matching shader directory, where ShaderProgram reads vs_main.bin and
 * fs_main.bin. A failed program load rolls the shader program back to nullptr.
 *
 * @param window Window that will present the rendered frames.
 * @return 0 on success, -1 if bgfx::init() or the shader program failed.
 */
int BGE::initBGFX(GLFWwindow *window) {
  bgfx::Init init;
  init.swapChain.width = W_WIDTH;
  init.swapChain.height = W_HEIGHT;
  init.swapChain.nwh =
      platformGetNativeWindowHandle((PlatformWindowHandle)window);
  init.reset = BGFX_RESET_VSYNC;
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

/**
 * @brief Releases shader, renderer, window and platform resources.
 *
 * Safe to call repeatedly or after a failed init(): each step checks whether
 * the corresponding resource exists before destroying it, and bgfx is only
 * shut down when #bgfxInitialized says it was started.
 *
 * @return 0 always.
 */
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
