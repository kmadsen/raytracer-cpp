#ifndef RECTANGLEPROGRAM_H_
#define RECTANGLEPROGRAM_H_

#include <GL/glew.h>

class RectangleProgram {
 public:
  RectangleProgram();
  virtual ~RectangleProgram();
  void draw();
 private:
  GLuint program;
  GLuint vaoHandle;
  GLuint vboHandles[2];
  GLuint vertShader;
  GLuint fragShader;
  GLuint frameBuffer;
};

#endif  // RECTANGLEPROGRAM_H_
