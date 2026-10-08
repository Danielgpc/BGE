#ifndef SHADERS_H
#define SHADERS_H

/**
 * @file shaders.h
 * @brief Shader program wrapper: loads compiled shader binaries and links them.
 */

#include <bgfx/bgfx.h>

/**
 * @brief Reads a vertex + fragment shader from disk and links them into a
 *        bgfx program.
 *
 * The inputs are the cross-compiled binaries produced by shaders/Makefile
 * (e.g. vs_main.bin / fs_main.bin for the active renderer backend). The
 * program is created with `true` as the third argument, so bgfx takes
 * ownership of the shader handles and destroys them with the program.
 *
 * @note If either file fails to load, the resulting program handle is invalid;
 *       check it with bgfx::isValid() before submitting draws.
 */
class ShaderProgram {
public:
  /**
   * @brief Loads both shader binaries and creates the linked program.
   *
   * @param vertexShaderFilename   Path to the compiled vertex shader binary.
   * @param fragmentShaderFilename Path to the compiled fragment shader binary.
   */
  ShaderProgram(const char *vertexShaderFilename,
                const char *fragmentShaderFilename);

  /**
   * @brief Returns the linked program handle used when submitting draws.
   *
   * @return The program handle; invalid if construction failed.
   */
  bgfx::ProgramHandle getProgramHandle() { return ShaderProgramHandle; }

  /** @brief Destroys the linked program (and its shaders) owned by bgfx. */
  ~ShaderProgram();

private:
  /** @brief Handle of the linked vertex + fragment program. */
  bgfx::ProgramHandle ShaderProgramHandle;

  /** @brief Handle of the loaded vertex shader binary. */
  bgfx::ShaderHandle VertexShader;

  /** @brief Handle of the loaded fragment shader binary. */
  bgfx::ShaderHandle FragmentShader;

  /**
   * @brief Reads a compiled shader file into memory and creates a bgfx shader.
   *
   * Reads the whole file in binary mode and hands a bgfx-owned copy of the
   * bytes to bgfx::createShader(). Failures are logged rather than thrown.
   *
   * @param filename Path to the compiled shader binary (e.g. "vs_main.bin").
   * @return The shader handle, or BGFX_INVALID_HANDLE if the file could not be
   *         opened or the shader was rejected by bgfx.
   */
  bgfx::ShaderHandle createShader(const char *filename);
};

#endif // !SHADERS_H
