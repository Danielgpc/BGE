#ifndef IMAGE_H
#define IMAGE_H

/**
 * @file image.h
 * @brief Image loading wrapper that turns an image file into a bgfx texture.
 */

#include <bgfx/bgfx.h>

/**
 * @brief Loads an image from disk and uploads it as a bgfx 2D texture.
 *
 * The file is decoded with stb_image and always converted to RGBA8 so the
 * shader can sample it directly. If loading or texture creation fails, the
 * object stays valid but its handle is BGFX_INVALID_HANDLE, which callers can
 * check with bgfx::isValid().
 *
 * @code
 *   Image img("../assets/texture.jpg");
 *   if (bgfx::isValid(img.getTextureHandle()))
 *       bgfx::setTexture(0, sampler, img.getTextureHandle());
 * @endcode
 */
class Image {
public:
  /**
   * @brief Loads @p filename and creates the GPU texture immediately.
   *
   * The path may be relative; the loader retries with a different number of
   * leading "../" segments because the working directory depends on how the
   * binary was launched.
   *
   * @param filename Path to the image file (jpg/png/etc., see stb_image).
   */
  Image(const char *filename);

  /** @brief Destroys the bgfx texture if one was successfully created. */
  ~Image();

  /**
   * @brief Returns the GPU texture handle for binding to a sampler uniform.
   *
   * @return The texture handle, or BGFX_INVALID_HANDLE if loading failed.
   */
  bgfx::TextureHandle getTextureHandle() { return textureHandle; }

private:
  /** @brief Handle of the texture created from the loaded image file. */
  bgfx::TextureHandle textureHandle;
};

#endif // !IMAGE_H
