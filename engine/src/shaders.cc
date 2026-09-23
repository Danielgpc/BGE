#include "shaders.h"
#include "bgfx/bgfx.h"
#include <Logger.h>

#include <fstream>
#include <iostream>
#include <vector>

ShaderProgram::ShaderProgram(const char *vertexShaderFilename,
                             const char *fragmentShaderFilename) {
  VertexShader = createShader(vertexShaderFilename);
  FragmentShader = createShader(fragmentShaderFilename);
  ShaderProgramHandle = bgfx::createProgram(VertexShader, FragmentShader, true);
}

ShaderProgram::~ShaderProgram() { bgfx::destroy(ShaderProgramHandle); }

bgfx::ShaderHandle ShaderProgram::createShader(const char *filename) {
  std::ifstream file(filename, std::ios::binary);
  if (!file) {
    LogError << "Failed to open shader: " << filename << std::endl;
    return BGFX_INVALID_HANDLE;
  }
  file.seekg(0, std::ios::end);
  std::streamsize fileSize = file.tellg();
  file.seekg(0, std::ios::beg);

  std::vector<char> fileBuffer(fileSize);

  file.read(fileBuffer.data(), fileSize);
  bgfx::ShaderHandle shader =
      bgfx::createShader(bgfx::copy(fileBuffer.data(), fileSize));
  if (!bgfx::isValid(shader)) {
    LogError << "Failed to create shader: " << filename << std::endl;
  }
  file.close();
  return shader;
}
