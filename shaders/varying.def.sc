// varying.def.sc - Attribute/varying declarations shared by the vertex and
// fragment shaders.
//
// bgfx uses this file to match the shader stages with the CPU-side
// bgfx::VertexLayout: a_* names must line up with the layout built in
// BGE::run(), and v_* names must be identical in both shader stages.
//
// NOTE: shaderc's varying parser only accepts //-style comments here.

// ---- Varyings (vertex shader -> fragment shader) ----
vec4 v_color0: COLOR0 = vec4(1.0, 0.0, 0.0, 1.0);
vec2 v_texcoord0: TEXCOORD0 = vec2(0.0, 0.0);

// ---- Vertex attributes (fed by the vertex buffer) ----
vec3 a_position: POSITION;
vec4 a_color0: COLOR0;
vec2 a_texcoord0: TEXCOORD0;
