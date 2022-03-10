#ifndef CAMERAPROGRAM_H_
#define CAMERAPROGRAM_H_

#include "RasterScene.h"

#include <GL/glew.h>

class CameraProgram {
 public:
  explicit CameraProgram(raster::Scene* _scene);
  virtual ~CameraProgram();
  void draw();
 private:
  raster::Scene* scene;

  GLuint program;
  GLuint vaoHandle;
  GLuint vboHandle;
  GLuint eboHandle;
  GLuint uboViewProjectionHandle;
  GLuint vertShader;
  GLuint fragShader;
};

#endif  // CAMERAPROGRAM_H_
