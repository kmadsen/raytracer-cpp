#ifndef CAMERAPROGRAM_H_
#define CAMERAPROGRAM_H_

#include "RasterScene.h"

#include <GL/glew.h>

class TriangleBuffer;

class CameraProgram {
 public:
  explicit CameraProgram(raster::Scene* _scene);
  virtual ~CameraProgram();
  void draw();
 private:
  raster::Scene* scene;

  TriangleBuffer* triangleBuffer;
  GLuint program;
  GLuint uboViewProjectionHandle;
  GLuint vertShader;
  GLuint fragShader;
};

#endif  // CAMERAPROGRAM_H_
