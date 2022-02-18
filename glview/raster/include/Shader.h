#ifndef SHADER_H_
#define SHADER_H_

#include <GL/glew.h>

#include <vector>

class Shader {
 public:
  Shader(void) {}
  ~Shader(void) {}

  static GLuint compileFile(const char* fileName, const GLuint& shaderType);
  static GLuint compileSource(const char* source, const GLuint& shaderType);

  static bool check(const GLuint& shader);

  static void printAttributes(GLuint programHandle);
  static void printActiveUniformVars(GLuint programHandle);

 private:
  static bool loadFile(const char* fileName, std::vector<GLchar> shaderCode);
};

#endif  // SHADER_H_
