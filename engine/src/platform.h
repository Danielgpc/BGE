#ifndef PLATFORM_H
#define PLATFORM_H

/**
 * @file platform.h
 * @brief Thin C API that hides the operating system behind a few functions.
 *
 * Each supported OS provides its own implementation (platform_mac.cc,
 * platform_linux.cc, platform_win.cc) with the same signatures, so the rest of
 * the engine can stay free of GLFW/Win32/Cocoa/X11 details. Only one of those
 * translation units is compiled per platform.
 */

#include "defines.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Opaque native window pointer.
 *
 * Backed by a GLFWwindow* internally, but exposed as void* so engine code does
 * not need to include GLFW headers to pass windows around.
 */
typedef void *PlatformWindowHandle;

/**
 * @brief Initializes the platform/windowing library (GLFW).
 *
 * @return Non-zero (b32 true) on success, 0 on failure.
 */
b32 platformInit(void);

/**
 * @brief Destroys any window still alive and shuts the platform library down.
 *
 * Safe to call multiple times; a window already destroyed through
 * platformDestroyWindow() is not destroyed again.
 */
void platformShutdown(void);

/**
 * @brief Creates the application window.
 *
 * @param width  Window width in pixels.
 * @param height Window height in pixels.
 * @param title  Null-terminated title bar string.
 * @return The new window handle, or nullptr if creation failed.
 */
PlatformWindowHandle platformCreateWindow(int width, int height, const char *title);

/**
 * @brief Destroys a window previously returned by platformCreateWindow().
 *
 * @param handle Window to destroy; nullptr is ignored. If it is the window
 *               tracked internally, that reference is cleared as well.
 */
void platformDestroyWindow(PlatformWindowHandle handle);

/**
 * @brief Changes the text shown in the window's title bar.
 *
 * @param handle Target window; nullptr is a no-op.
 * @param title  New null-terminated title string.
 */
void platformSetWindowTitle(PlatformWindowHandle handle, const char *title);

/**
 * @brief Checks whether the user asked to close the window.
 *
 * @param handle Window to query.
 * @return Non-zero when the window should close; a null handle is treated as
 *         "should close" so the main loop cannot spin forever.
 */
b32 platformWindowShouldClose(PlatformWindowHandle handle);

/**
 * @brief Processes all pending input/window events (non-blocking).
 *
 * Must be called once per frame so callbacks (key presses, close button) run.
 */
void platformPollEvents(void);

/**
 * @brief Returns a monotonic high-resolution clock in seconds.
 *
 * @return Seconds since an unspecified fixed origin; suitable for frame timing.
 */
f64 platformGetTime(void);

/**
 * @brief Returns the OS-level window object expected by the graphics API.
 *
 * Cocoa NSWindow* on macOS, X11 Window (cast to void*) on Linux and HWND on
 * Windows - i.e. exactly what bgfx's swapChain.nwh field needs.
 *
 * @param handle Window to query.
 * @return Native handle, or nullptr for a null/invalid window.
 */
void *platformGetNativeWindowHandle(PlatformWindowHandle handle);

#ifdef __cplusplus
}
#endif

#endif // PLATFORM_H
