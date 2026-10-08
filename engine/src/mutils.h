#ifndef MUTILS_H
#define MUTILS_H

/**
 * @file mutils.h
 * @brief Small math/utility helpers shared by the engine, notably the vertex
 *        layout used by the mesh data.
 */

#include "defines.h"
#include "glm/ext/vector_float3.hpp"
#include <glm/glm.hpp>

/**
 * @brief Namespace grouping engine-side math helpers and POD data structures.
 */
namespace mutils {
/**
 * @brief A single vertex sent to the GPU.
 *
 * Field order matters: it must stay in sync with the bgfx::VertexLayout built
 * in BGE::run() (Position -> Color0 -> TexCoord0), otherwise the shader will
 * read misaligned attributes.
 */
struct Vertex {
  /** @brief Object-space position of the vertex. */
  glm::vec3 position;

  /** @brief Packed ABGR vertex color (0xAABBGGRR style, 4x8-bit). */
  u32 color;

  /** @brief Texture coordinates in the range [0, 1]. */
  glm::vec2 TextureCoordinates;
};
} // namespace mutils

#endif // !MUTILS_H
