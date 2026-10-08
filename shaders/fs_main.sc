/**
 * @file fs_main.sc
 * @brief Fragment shader: combines the vertex color with the sampled texture.
 *
 * Compiled by shaders/Makefile into per-backend binaries (e.g. fs_main.bin).
 */

/** @brief Interpolated values received from the vertex shader. */
$input v_color0, v_texcoord0

#include <bgfx_shader.sh>

/** @brief Sampler bound to texture unit 0 (the "textureColor" uniform). */
SAMPLER2D(textureColor, 0);

/**
 * @brief Entry point executed once per fragment.
 *
 * Multiplies the interpolated vertex color by the texture sample, which lets
 * a white vertex color (0xffffffff) show the texture unmodified while still
 * allowing tinting by changing the vertex color.
 */
void main() {
  gl_FragColor = v_color0 * texture2D(textureColor, v_texcoord0);
}
