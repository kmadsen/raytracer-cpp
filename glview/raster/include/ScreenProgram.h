#ifndef SCREENPROGRAM_H_
#define SCREENPROGRAM_H_

#include <GL/glew.h>

class ScreenProgram {
 public:
  ScreenProgram();
  virtual ~ScreenProgram();
  void draw();
 private:
  GLuint program;
  GLuint vaoHandle;
  GLuint vboHandle;
  GLuint eboHandle;
  GLuint vertShader;
  GLuint fragShader;
};

#endif  // SCREENPROGRAM_H_
