/**
 * @file main.cc
 * @brief Entry point of the game executable.
 *
 * Boots the engine and drives its lifecycle: init -> run -> shutdown. Any
 * initialization failure exits immediately with a non-zero status so the
 * process never reaches the render loop half-initialized.
 */

#include <bge.h>

/**
 * @brief Creates the engine, runs the main loop and shuts it down.
 *
 * @return 0 on normal exit, 1 if BGE::init() failed (window/renderer startup).
 */
int main() {
  /** @brief The engine instance that owns the window, renderer and main loop. */
  BGE instance;
  if (instance.init() != 0)
    return 1;
  instance.run();
  instance.shutdown();
  return 0;
}
