/**
 * @file image.cc
 * @brief Image decoding and texture upload implementation.
 */

#include "image.h"
#include "bgfx/bgfx.h"
#include "bgfx/defines.h"
#include "defines.h"
#include "logging.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <string>

// stbi_load returns nullptr if the file can't be opened or parsed. The path
// is relative and the CWD depends on how the binary was launched (make run ->
// game/, ./build/game/bgeTestGame -> build/game/, etc.), so retry with
// adjusted depth before giving up.

/**
 * @brief Loads an image file, retrying with shorter/longer relative paths.
 *
 * stb_image always converts the result to 4 channels (RGBA) so the data can be
 * uploaded as an RGBA8 texture without further swizzling. Because the working
 * directory is not known in advance, the original path is first stripped of up
 * to three leading "../" segments, and then re-prefixed with "../" up to three
 * times; the first attempt that yields a decodable file wins.
 *
 * @param filename Relative path to attempt first (e.g. "../assets/texture.jpg").
 * @param[out] width  Receives the image width in pixels, or 0 on total failure.
 * @param[out] height Receives the image height in pixels, or 0 on total failure.
 * @return Heap buffer of RGBA pixels owned by the caller (free with
 *         stbi_image_free()), or nullptr if no candidate path could be loaded.
 */
static u8 *loadImageData(const char *filename, i32 &width, i32 &height) {
  width = 0;
  height = 0;

  u8 *data = stbi_load(filename, &width, &height, nullptr, STBI_rgb_alpha);
  if (data)
    return data;

  // Try dropping leading "../" segments first (the binary may run one level
  // deeper than the path assumes).
  std::string path = filename;
  for (int i = 0; i < 3; ++i) {
    if (path.rfind("../", 0) != 0)
      break;
    path = path.substr(3);
    data = stbi_load(path.c_str(), &width, &height, nullptr, STBI_rgb_alpha);
    if (data)
      return data;
  }

  // Then try adding "../" prefixes (the binary may run one level shallower).
  path = filename;
  for (int i = 0; i < 3; ++i) {
    path = "../" + path;
    data = stbi_load(path.c_str(), &width, &height, nullptr, STBI_rgb_alpha);
    if (data)
      return data;
  }

  return nullptr;
}

/**
 * @brief Decodes @p filename and uploads it as a bgfx RGBA8 texture.
 *
 * On decode failure the handle is set to BGFX_INVALID_HANDLE and an error is
 * logged; the object remains usable and getTextureHandle() can still be called
 * (guarded with bgfx::isValid()). On success the CPU-side pixels are freed
 * right after bgfx::copy() has taken ownership of them.
 *
 * @param filename Path of the image to load (relative paths are retried, see
 *                 loadImageData()).
 */
Image::Image(const char *filename) {
  i32 width = 0, height = 0;
  u8 *data = loadImageData(filename, width, height);
  if (!data) {
    LogError("Failed to load image (file not found or invalid): {}", filename);
    textureHandle = BGFX_INVALID_HANDLE;
    return;
  }

  const u32 size = (u32)width * (u32)height * 4;
  textureHandle = bgfx::createTexture2D(
      (u16)width, (u16)height, false, 1, bgfx::TextureFormat::RGBA8,
      BGFX_SAMPLER_POINT | BGFX_SAMPLER_UVW_CLAMP, bgfx::copy(data, size));
  stbi_image_free(data);

  if (!bgfx::isValid(textureHandle)) {
    LogError("Failed to create texture: {}", filename);
  }
}

/**
 * @brief Destroys the GPU texture created by the constructor.
 *
 * Does nothing when the texture was never created (loading failed), so it is
 * always safe to call.
 */
Image::~Image() {
  if (bgfx::isValid(textureHandle)) {
    bgfx::destroy(textureHandle);
  }
}
