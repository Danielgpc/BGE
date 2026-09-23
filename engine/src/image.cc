#include "image.h"
#include "bgfx/bgfx.h"
#include "bgfx/defines.h"
#include "defines.h"
#include <Logger.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <string>

// stbi_load returns nullptr if the file can't be opened or parsed. The path
// is relative and the CWD depends on how the binary was launched (make run ->
// game/, ./build/game/bgeTestGame -> build/game/, etc.), so retry with
// adjusted depth before giving up.
static u8 *loadImageData(const char *filename, i32 &width, i32 &height) {
  width = 0;
  height = 0;

  u8 *data = stbi_load(filename, &width, &height, nullptr, STBI_rgb_alpha);
  if (data)
    return data;

  std::string path = filename;
  for (int i = 0; i < 3; ++i) {
    if (path.rfind("../", 0) != 0)
      break;
    path = path.substr(3);
    data = stbi_load(path.c_str(), &width, &height, nullptr, STBI_rgb_alpha);
    if (data)
      return data;
  }

  path = filename;
  for (int i = 0; i < 3; ++i) {
    path = "../" + path;
    data = stbi_load(path.c_str(), &width, &height, nullptr, STBI_rgb_alpha);
    if (data)
      return data;
  }

  return nullptr;
}

Image::Image(const char *filename) {
  i32 width = 0, height = 0;
  u8 *data = loadImageData(filename, width, height);
  if (!data) {
    LogError << "Failed to load image (file not found or invalid): "
             << filename << std::endl;
    textureHandle = BGFX_INVALID_HANDLE;
    return;
  }

  const u32 size = (u32)width * (u32)height * 4;
  textureHandle = bgfx::createTexture2D(
      (u16)width, (u16)height, false, 1, bgfx::TextureFormat::RGBA8,
      BGFX_SAMPLER_POINT | BGFX_SAMPLER_UVW_CLAMP, bgfx::copy(data, size));
  stbi_image_free(data);

  if (!bgfx::isValid(textureHandle)) {
    LogError << "Failed to create texture: " << filename << std::endl;
  }
}

Image::~Image() {
  if (bgfx::isValid(textureHandle)) {
    bgfx::destroy(textureHandle);
  }
}
