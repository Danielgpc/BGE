// fs_main.sc - Fragment shader: combines the vertex color with the sampled
// texture.
// Compiled by shaders/Makefile into per-backend binaries (fs_main.bin).

// Interpolated values received from the vertex shader.
$input v_color0, v_texcoord0

#include <bgfx_shader.sh>

// Sampler bound to texture unit 0 (the "textureColor" uniform).
SAMPLER2D(textureColor, 0);

// Entry point executed once per fragment. Multiplies the interpolated vertex
// color by the texture sample, so a white vertex color (0xffffffff) shows the
// texture unmodified while tinting is still possible via the vertex color.
void main() {
  gl_FragColor = v_color0 * texture2D(textureColor, v_texcoord0);
}
