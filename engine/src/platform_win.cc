#include "platform.h"

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

static GLFWwindow *g_window = nullptr;

b32 platformInit(void) {
    return glfwInit() == GLFW_TRUE;
}

void platformShutdown(void) {
    if (g_window) {
        glfwDestroyWindow(g_window);
        g_window = nullptr;
    }
    glfwTerminate();
}

PlatformWindowHandle platformCreateWindow(int width, int height, const char *title) {
    g_window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    return (PlatformWindowHandle)g_window;
}

void platformDestroyWindow(PlatformWindowHandle handle) {
    GLFWwindow *window = (GLFWwindow *)handle;
    if (window) {
        glfwDestroyWindow(window);
        if (window == g_window) {
            g_window = nullptr;
        }
    }
}

void platformSetWindowTitle(PlatformWindowHandle handle, const char *title) {
    GLFWwindow *window = (GLFWwindow *)handle;
    if (window) {
        glfwSetWindowTitle(window, title);
    }
}

b32 platformWindowShouldClose(PlatformWindowHandle handle) {
    GLFWwindow *window = (GLFWwindow *)handle;
    return window ? glfwWindowShouldClose(window) : GLFW_TRUE;
}

void platformPollEvents(void) {
    glfwPollEvents();
}

f64 platformGetTime(void) {
    return glfwGetTime();
}

void *platformGetNativeWindowHandle(PlatformWindowHandle handle) {
    GLFWwindow *window = (GLFWwindow *)handle;
    if (!window) return nullptr;
    return (void *)glfwGetWin32Window(window);
}