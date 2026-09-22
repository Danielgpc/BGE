#ifndef MUTILS_H
#define MUTILS_H

#include "defines.h"
#include "glm/ext/vector_float3.hpp"
#include <glm/glm.hpp>
namespace mutils {
struct Vertex {
  glm::vec3 position;
  u32 color;
  glm::vec2 TextureCoordinates;
};
} // namespace mutils

#endif // !MUTILS_H
