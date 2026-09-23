#ifndef BGE_H
#define BGE_H

#include <GLFW/glfw3.h>
#include <bgfx/bgfx.h>

class ShaderProgram;

class BGE {
public:
  BGE();
  ~BGE();
  int init();
  int run();
  int shutdown();

private:
  GLFWwindow *initWindow();
  GLFWwindow *window = nullptr;
  bool bgfxInitialized = false;
  ShaderProgram *shaderProgram = nullptr;
  int initBGFX(GLFWwindow *window);
};

#endif // !BGE_H
