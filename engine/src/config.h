#ifndef CONFIG_H
#define CONFIG_H

/**
 * @file config.h
 * @brief Compile-time window configuration used when creating the main window.
 *
 * These macros are read by BGE::init() when it asks the platform layer to
 * create the application window. Change them here and rebuild the engine to
 * alter the default window setup.
 */

/** @brief Height, in pixels, of the application window (used at creation time). */
#define W_HEIGHT 1080

/** @brief Width, in pixels, of the application window (used at creation time). */
#define W_WIDTH 1920

/** @brief Title bar text shown on the application window. */
#define W_TITLE "BGE"

#endif // !CONFIG_H
