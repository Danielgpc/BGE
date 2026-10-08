/**
 * @file varying.def.sc
 * @brief Attribute/varying declarations shared by the vertex and fragment
 *        shaders.
 *
 * bgfx uses this file to match the shader stages with the CPU-side
 * bgfx::VertexLayout: a_* names must line up with the layout built in
 * BGE::run(), and v_* names must be identical in both shader stages.
 */

// ---- Varyings (vertex shader -> fragment shader) ----

/** @brief Interpolated vertex color, defaults to opaque red. */
vec4 v_color0: COLOR0 = vec4(1.0, 0.0, 0.0, 1.0);
/** @brief Interpolated texture coordinates, defaults to (0, 0). */
vec2 v_texcoord0: TEXCOORD0 = vec2(0.0, 0.0);

// ---- Vertex attributes (fed by the vertex buffer) ----

/** @brief Vertex position, supplied as a float3. */
vec3 a_position: POSITION;
/** @brief Packed vertex color, supplied as 4x uint8 normalized to [0, 1]. */
vec4 a_color0: COLOR0;
/** @brief Vertex texture coordinates, supplied as a float2. */
vec2 a_texcoord0: TEXCOORD0;
