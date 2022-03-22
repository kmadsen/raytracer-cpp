#include "TriangleBuffer.h"

#include <glm/geometric.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "CameraProgram.h"
#include "Shader.h"

static TriangleBufferRow createRow(const glm::vec3& vertex,
                                   const glm::vec3& normal,
                                   const raster::Material* material) {
  return {.vertex = vertex,
          .normal = normal,
          .color = material->color,
          .kAmbient = material->kAmbient,
          .kDiffuse = material->kDiffuse};
}

static glm::vec3 triangleNormal(const raster::Triangle& triangle) {
  auto e0 = triangle.p1 - triangle.p0;
  auto e1 = triangle.p2 - triangle.p0;
  glm::vec3 normal = glm::cross(e0, e1);
  return glm::normalize(normal);
}

TriangleBuffer::TriangleBuffer(const std::vector<raster::Triangle*> triangles) {
  triangleRowCount = triangles.size() * 3;
  rows = std::vector<TriangleBufferRow>(triangleRowCount);
  indices = std::vector<int>(triangleRowCount);
  int rowIndex = 0;
  int indicesIndex = 0;
  for (auto triangle : triangles) {
    auto triangleRef = *triangle;
    auto normal = triangleNormal(triangleRef);
    indices[indicesIndex++] = rowIndex;
    rows[rowIndex++] = createRow(triangleRef.p0, normal, triangleRef.material);
    indices[indicesIndex++] = rowIndex;
    rows[rowIndex++] = createRow(triangleRef.p1, normal, triangleRef.material);
    indices[indicesIndex++] = rowIndex;
    rows[rowIndex++] = createRow(triangleRef.p2, normal, triangleRef.material);
  }
}

TriangleBuffer::~TriangleBuffer() {
  printf("delete TriangleBuffer\n");
  glDeleteBuffers(1, &vboHandle);
  glDeleteBuffers(1, &eboHandle);
  glDeleteVertexArrays(1, &vaoHandle);
}

void TriangleBuffer::bind() {
  glGenVertexArrays(1, &vaoHandle);
  glGenBuffers(1, &vboHandle);
  glGenBuffers(1, &eboHandle);

  size_t vaoSize = sizeof(TriangleBufferRow) * triangleRowCount;
  const float* vaoValues = &(rows[0].vertex.x);

  size_t eboSize = sizeof(int) * triangleRowCount;
  const int* eboValues = &(indices[0]);

  printf("size of values %x+%d %x+%d\n", vaoValues, vaoSize, eboValues,
         eboSize);

  glBindVertexArray(vaoHandle);

  glBindBuffer(GL_ARRAY_BUFFER, vboHandle);
  glBufferData(GL_ARRAY_BUFFER, vaoSize, vaoValues, GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboHandle);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, eboSize, eboValues, GL_STATIC_DRAW);

  // Position attribute
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), 0);
  // Normal attribute
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat),
                        reinterpret_cast<void*>(3 * sizeof(GLfloat)));
  // Color attribute
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat),
                        reinterpret_cast<void*>(6 * sizeof(GLfloat)));
  // Ambient attribute
  glEnableVertexAttribArray(3);
  glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat),
                        reinterpret_cast<void*>(9 * sizeof(GLfloat)));
  // Diffuse attribute
  glEnableVertexAttribArray(4);
  glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat),
                        reinterpret_cast<void*>(10 * sizeof(GLfloat)));

  glBindVertexArray(0);
}

void TriangleBuffer::printDebug() {
  printf(
      "|-MemAddress-|---Vertex [x, y, z]---|---Normal [x, y, z]---|---Material "
      "[r, g, b, ka, kd]---|\n");
  float* flat_array = &(rows[0].vertex.x);
  int rowSize = sizeof(TriangleBufferRow) / sizeof(float);
  for (int i = 0; i < rows.size(); i++) {
    printf("[%x] ", &flat_array[i * rowSize]);
    for (int j = 0; j < rowSize; j++) {
      printf("%ff, ", j, flat_array[i * rowSize + j]);
    }
    printf("\n");
  }

  printf("|-MemAddress-|---Vertex indices---|\n");
  for (int i = 0; i < indices.size();) {
    printf("[%x] ", &indices[i]);
    printf("%d, ", indices[i++]);
    printf("%d, ", indices[i++]);
    printf("%d\n", indices[i++]);
  }
  printf("\n");

  printf("printRowsAndIndices rows %x %d\n", &(rows[0].vertex.x), rows.size());
  printf("printRowsAndIndices indices %x %d\n", &(indices[0]), indices.size());
}

void TriangleBuffer::draw() {
  glBindVertexArray(vaoHandle);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboHandle);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
