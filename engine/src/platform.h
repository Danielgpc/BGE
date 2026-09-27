#ifndef PLATFORM_H
#define PLATFORM_H

#include "defines.h"

#ifdef __cplusplus
extern "C" {
#endif

// Platform-agnostic native window handle type
typedef void *PlatformWindowHandle;

// Platform initialization/shutdown
b32 platformInit(void);
void platformShutdown(void);

// Window management
PlatformWindowHandle platformCreateWindow(int width, int height, const char *title);
void platformDestroyWindow(PlatformWindowHandle handle);
void platformSetWindowTitle(PlatformWindowHandle handle, const char *title);
b32 platformWindowShouldClose(PlatformWindowHandle handle);
void platformPollEvents(void);

// Time
f64 platformGetTime(void);

// Native window handle for graphics APIs (bgfx, etc.)
void *platformGetNativeWindowHandle(PlatformWindowHandle handle);

#ifdef __cplusplus
}
#endif

#endif // PLATFORM_H