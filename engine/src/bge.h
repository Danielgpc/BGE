#ifndef BGE_H
#define BGE_H

/**
 * @file bge.h
 * @brief Public interface of the BGE engine.
 *
 * BGE owns the whole engine lifecycle: it creates the window, initializes the
 * bgfx renderer, runs the main loop and finally releases every resource.
 * A host application only needs to construct a BGE object and call init(),
 * run() and shutdown() in that order.
 */

#include <GLFW/glfw3.h>
#include <bgfx/bgfx.h>

#include "defines.h"

/** @brief Forward declaration; the full definition lives in shaders.h. */
class ShaderProgram;

/**
 * @brief Top level engine facade: window + bgfx renderer + main loop.
 *
 * Typical usage:
 * @code
 *   BGE engine;
 *   if (engine.init() != 0) return 1;
 *   engine.run();
 *   engine.shutdown();
 * @endcode
 *
 * @note The object must be destroyed only after shutdown() has been called, and
 *       init() must be called before run().
 */
class BGE_API BGE {
public:
  /** @brief Constructs an uninitialized engine (all members are defaulted). */
  BGE();

  /** @brief Destructor; does not release resources, call shutdown() explicitly. */
  ~BGE();

  /**
   * @brief Initializes the platform layer, the window and bgfx.
   *
   * Order of operations: platform init -> window creation -> bgfx init and
   * shader loading. On any failure the partially initialized subsystems are
   * rolled back before returning.
   *
   * @return 0 on success, -1 if the platform, window or bgfx failed to start.
   */
  int init();

  /**
   * @brief Runs the main loop until the window is asked to close.
   *
   * Creates the geometry buffers (index + vertex) and the texture uniform,
   * then loops: poll events, update the FPS title, clear the screen, draw the
   * textured quad and flip bgfx's frame queue. All GPU resources created here
   * are destroyed before returning.
   *
   * @return 0 on success, -1 if no shader program is available or buffer
   *         creation failed.
   */
  int run();

  /**
   * @brief Releases every resource acquired by init() and run().
   *
   * Destroys the shader program, shuts down bgfx (if it was initialized),
   * destroys the window and terminates the platform layer. Safe to call even
   * if init() failed halfway.
   *
   * @return 0 always; provided for symmetry with init()/run().
   */
  int shutdown();

private:
  /**
   * @brief Creates the platform window and stores it in #window.
   *
   * @return The native window handle, or nullptr if creation failed.
   */
  GLFWwindow *initWindow();

  /** @brief Native GLFW window owned by the engine; nullptr until init() succeeds. */
  GLFWwindow *window = nullptr;

  /** @brief True once bgfx::init() succeeded; guards against a double shutdown. */
  bool bgfxInitialized = false;

  /** @brief Engine-wide shader program; created in initBGFX(), deleted in shutdown(). */
  ShaderProgram *shaderProgram = nullptr;

  /**
   * @brief Initializes bgfx against @p window and loads the shader program.
   *
   * Builds a bgfx::Init with the window's native handle and the window size,
   * then picks the shader directory matching the renderer bgfx selected
   * (see shaderAPIDir()). The program is validated before returning.
   *
   * @param window The GLFW window that will host the swap chain.
   * @return 0 on success, -1 if bgfx or the shader program failed to load.
   */
  int initBGFX(GLFWwindow *window);
};

#endif // !BGE_H
