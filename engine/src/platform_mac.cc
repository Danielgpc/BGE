/**
 * @file platform_mac.cc
 * @brief macOS implementation of the platform API (GLFW + Cocoa native handle).
 *
 * Compiled only on Apple; supplies the functions declared in platform.h using
 * GLFW for windowing and glfwGetCocoaWindow() for the handle bgfx needs.
 */

#include "platform.h"

#define GLFW_EXPOSE_NATIVE_COCOA
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

/**
 * @brief Window created by platformCreateWindow(), kept so platformShutdown()
 *        can destroy it even if the caller forgot to.
 */
static GLFWwindow *g_window = nullptr;

/**
 * @brief Initializes GLFW.
 *
 * @return b32 true if GLFW started, b32 false otherwise.
 */
b32 platformInit(void) {
    return glfwInit() == GLFW_TRUE;
}

/**
 * @brief Destroys the internally tracked window and terminates GLFW.
 *
 * Called by the engine during shutdown(); harmless to call twice.
 */
void platformShutdown(void) {
    if (g_window) {
        glfwDestroyWindow(g_window);
        g_window = nullptr;
    }
    glfwTerminate();
}

/**
 * @brief Creates a GLFW window and remembers it in g_window.
 *
 * @param width  Width in pixels.
 * @param height Height in pixels.
 * @param title  Title bar text.
 * @return Opaque handle to the new window, or nullptr on failure.
 */
PlatformWindowHandle platformCreateWindow(int width, int height, const char *title) {
    g_window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    return (PlatformWindowHandle)g_window;
}

/**
 * @brief Destroys a window and clears the global pointer if it matches.
 *
 * @param handle Window to destroy; nullptr is ignored.
 */
void platformDestroyWindow(PlatformWindowHandle handle) {
    GLFWwindow *window = (GLFWwindow *)handle;
    if (window) {
        glfwDestroyWindow(window);
        if (window == g_window) {
            g_window = nullptr;
        }
    }
}

/**
 * @brief Sets the window title text.
 *
 * @param handle Target window; nullptr is a no-op.
 * @param title  New title string.
 */
void platformSetWindowTitle(PlatformWindowHandle handle, const char *title) {
    GLFWwindow *window = (GLFWwindow *)handle;
    if (window) {
        glfwSetWindowTitle(window, title);
    }
}

/**
 * @brief Tells the main loop whether the window was closed by the user.
 *
 * @param handle Window to query.
 * @return b32 true when it should close; a null handle also returns true so a
 *         lost window cannot cause an endless loop.
 */
b32 platformWindowShouldClose(PlatformWindowHandle handle) {
    GLFWwindow *window = (GLFWwindow *)handle;
    return window ? glfwWindowShouldClose(window) : GLFW_TRUE;
}

/**
 * @brief Processes pending OS events (input, resize, close requests).
 */
void platformPollEvents(void) {
    glfwPollEvents();
}

/**
 * @brief Current time in seconds from GLFW's monotonic clock.
 *
 * @return Seconds elapsed since GLFW initialization.
 */
f64 platformGetTime(void) {
    return glfwGetTime();
}

/**
 * @brief Returns the Cocoa NSWindow* backing the GLFW window.
 *
 * @param handle Window to query; nullptr yields nullptr.
 * @return Native NSWindow pointer for bgfx's swap chain, or nullptr.
 */
void *platformGetNativeWindowHandle(PlatformWindowHandle handle) {
    GLFWwindow *window = (GLFWwindow *)handle;
    if (!window) return nullptr;
    return (void *)glfwGetCocoaWindow(window);
}
