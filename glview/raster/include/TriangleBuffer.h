#ifndef TRIANGLEBUFFER_H_
#define TRIANGLEBUFFER_H_

#include <GL/glew.h>

#include <vector>
#include <glm/glm.hpp>

#include "RasterScene.h"

typedef struct {
  glm::vec3 vertex;
  glm::vec3 normal;
  glm::vec3 color;
  float kAmbient;
  float kDiffuse;
} TriangleBufferRow;

class TriangleBuffer {
 private:
  int triangleRowCount;
  GLuint vboHandle;
  GLuint vaoHandle;
  GLuint eboHandle;
  std::vector<TriangleBufferRow> rows;
  std::vector<int> indices;

 public:
  explicit TriangleBuffer(const std::vector<raster::Triangle*> triangles);
  ~TriangleBuffer();

  void bind();
  void draw();

  void printDebug();
};

#endif  // TRIANGLEBUFFER_H_
