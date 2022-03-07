#ifndef TRIANGLEPROGRAM_H_
#define TRIANGLEPROGRAM_H_

#include <GL/glew.h>

class TriangleProgram {
 public:
  TriangleProgram();
  virtual ~TriangleProgram();
  void draw();
 private:
  GLuint program;
  GLuint vaoHandle;
  GLuint vboHandles[2];
  GLuint vertShader;
  GLuint fragShader;
};

#endif  // TRIANGLEPROGRAM_H_
