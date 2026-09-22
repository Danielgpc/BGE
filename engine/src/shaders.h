#ifndef SHADERS_H
#define SHADERS_H

#include <bgfx/bgfx.h>

class ShaderProgram {
public:
  ShaderProgram(const char *vertexShaderFilename,
                const char *fragmentShaderFilename);
  bgfx::ProgramHandle getProgramHandle() { return ShaderProgramHandle; }
  ~ShaderProgram();

private:
  bgfx::ProgramHandle ShaderProgramHandle;
  bgfx::ShaderHandle VertexShader;
  bgfx::ShaderHandle FragmentShader;

  bgfx::ShaderHandle createShader(const char *filename);
};

#endif // !SHADERS_H
