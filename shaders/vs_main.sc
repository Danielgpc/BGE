// vs_main.sc - Vertex shader: transforms vertices and forwards color/UVs
// to the fragment stage.
// Compiled by shaders/Makefile into per-backend binaries (vs_main.bin).

// Vertex attributes consumed from the vertex buffer.
$input a_position, a_color0, a_texcoord0
// Values written out for the fragment shader to interpolate.
$output v_color0, v_texcoord0

#include <bgfx_shader.sh>

// Entry point executed once per vertex. Projects the object-space position
// with bgfx's combined model/view/projection matrix, then passes the vertex
// color and texture coordinates along as varyings for rasterization.
void main() {
  gl_Position = mul(u_modelViewProj, vec4(a_position, 1.0));
  v_color0 = a_color0;
  v_texcoord0 = a_texcoord0;
}
