#ifndef IMAGE_H
#define IMAGE_H

#include <bgfx/bgfx.h>

class Image {
public:
  Image(const char *filename);
  ~Image();

  bgfx::TextureHandle getTextureHandle() { return textureHandle; }

private:
  bgfx::TextureHandle textureHandle;
};

#endif // !IMAGE_H
