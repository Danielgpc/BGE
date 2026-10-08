/**
 * @file shaders.cc
 * @brief Loading of compiled shader binaries and program linking.
 */

#include "shaders.h"
#include "bgfx/bgfx.h"
#include "logging.h"

#include <fstream>
#include <iostream>
#include <vector>

/**
 * @brief Loads both shader binaries and links them into one bgfx program.
 *
 * The program is created with `true` for the third parameter, meaning bgfx
 * takes ownership of the two shader handles and destroys them when the program
 * is destroyed. If a shader file is missing, its handle is invalid and bgfx
 * will produce an invalid program - callers verify this with
 * bgfx::isValid(getProgramHandle()).
 *
 * @param vertexShaderFilename   Path to the compiled vertex shader binary.
 * @param fragmentShaderFilename Path to the compiled fragment shader binary.
 */
ShaderProgram::ShaderProgram(const char *vertexShaderFilename,
                             const char *fragmentShaderFilename) {
  VertexShader = createShader(vertexShaderFilename);
  FragmentShader = createShader(fragmentShaderFilename);
  ShaderProgramHandle = bgfx::createProgram(VertexShader, FragmentShader, true);
}

/**
 * @brief Destroys the linked program (and, per the flag above, its shaders).
 */
ShaderProgram::~ShaderProgram() { bgfx::destroy(ShaderProgramHandle); }

/**
 * @brief Reads a compiled shader file and wraps its bytes in a bgfx shader.
 *
 * The file is read in binary mode in one go (size first, then contents). The
 * bytes are duplicated into bgfx-owned memory with bgfx::copy(), so the local
 * buffer can go out of scope immediately after. Errors are logged and reported
 * through the returned handle instead of aborting.
 *
 * @param filename Path to the compiled shader binary produced by the shader
 *                 build (e.g. "build/shaders/metal/vs_main.bin").
 * @return Valid shader handle on success, BGFX_INVALID_HANDLE if the file
 *         cannot be opened or bgfx rejects the binary.
 */
bgfx::ShaderHandle ShaderProgram::createShader(const char *filename) {
  std::ifstream file(filename, std::ios::binary);
  if (!file) {
    LogError("Failed to open shader: {}", filename);
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
    LogError("Failed to create shader: {}", filename);
  }
  file.close();
  return shader;
}
