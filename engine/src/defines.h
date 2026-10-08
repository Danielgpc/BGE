#ifndef DEFINES_H
#define DEFINES_H

/**
 * @file defines.h
 * @brief Fixed-width integer/float typedefs, boolean type and DLL export
 *        macros used everywhere in the engine.
 *
 * Keeping these in one header avoids <cstdint>-style verbosity and gives the
 * codebase a consistent set of names (u32, i64, f32, b32, ...).
 */

// Unsigned int types.

/** @brief Unsigned 8-bit integer */
typedef unsigned char u8;

/** @brief Unsigned 16-bit integer */
typedef unsigned short u16;

/** @brief Unsigned 32-bit integer */
typedef unsigned int u32;

/** @brief Unsigned 64-bit integer */
typedef unsigned long long u64;

// Signed int types.

/** @brief Signed 8-bit integer */
typedef signed char i8;

/** @brief Signed 16-bit integer */
typedef signed short i16;

/** @brief Signed 32-bit integer */
typedef signed int i32;

/** @brief Signed 64-bit integer */
typedef signed long long i64;

// Floating point types

/** @brief 32-bit floating point number */
typedef float f32;

/** @brief 64-bit floating point number */
typedef double f64;

// Boolean types

/**
 * @brief 32-bit boolean type, used for APIs which require it.
 *
 * Convention: 0 means false, any non-zero value means true.
 */
typedef int b32;

// Shared library export/import macros

/**
 * @def BGE_API
 * @brief Marks symbols that must be visible outside the engine shared library.
 *
 * Expands to dllexport when the engine itself is being built
 * (BGE_ENGINE_EXPORTS is defined by the engine Makefile), to dllimport for
 * consumers of the DLL on Windows, and to default visibility elsewhere. It
 * decorates public classes such as BGE so the game can link against them.
 */
#if defined(_WIN32)
  #if defined(BGE_ENGINE_EXPORTS)
    #define BGE_API __declspec(dllexport)
  #else
    #define BGE_API __declspec(dllimport)
  #endif
#else
  #define BGE_API __attribute__((visibility("default")))
#endif

#endif // !DEFINES_H
