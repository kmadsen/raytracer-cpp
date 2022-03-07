#include "TriangleProgram.h"
#include "Shader.h"

#include <GLFW/glfw3.h>

TriangleProgram::TriangleProgram() {
  /*
  * geometry to use. these are 3 xyz points (9 floats total) to make a triangle
  */
  GLfloat points[] = {
    0.0f, 0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f
  };
  GLfloat colors[] = {
    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f
  };

  // Vertex buffer object (VBO). Generate buffers handle's to pass point and
  // color data to the vertex shader.
  glGenBuffers(2, vboHandles);
  GLuint pointsHandle = vboHandles[0];
  GLuint colorsHandle = vboHandles[1];

  // Bind the points and colors to the handles.
  glBindBuffer(GL_ARRAY_BUFFER, pointsHandle);
  glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), points, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, colorsHandle);
  glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), colors, GL_STATIC_DRAW);

  /*
  * Vertex array object (VAO) is a little descriptor that defines which
  * data from vertex buffer objects should be used as input variables to vertex
  * shaders. in our case - use our only VBO, and say 'every three floats is a
  * variable'
  */
  glGenVertexArrays(1, &vaoHandle);
  glBindVertexArray(vaoHandle);

  glEnableVertexAttribArray(0);  // Vertex points
  glEnableVertexAttribArray(1);  // Vertex colors

  glBindBuffer(GL_ARRAY_BUFFER, pointsHandle);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

  glBindBuffer(GL_ARRAY_BUFFER, colorsHandle);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
  glBindVertexArray(0);

  /*
  * These are the strings of code for the shaders
  * the vertex shader positions each vertex point
  */
  static const char* vert_source = R"(
    #version 410

    layout (location=0) in vec3 vertexPosition;
    layout (location=1) in vec3 vertexColor;

    layout (location=0) out vec3 vColor;

    void main () {
      vColor = vertexColor;
      gl_Position = vec4(vertexPosition, 1.0);
    };
  )";

  /*
  * The fragment shader colours each fragment (pixel-sized area of the triangle)
  */
  const char* frag_source = R"(
    #version 410

    layout (location=0) in vec3 vColor;
    layout (location=0) out vec4 fragmentColor;

    void main () {
      fragmentColor = vec4(vColor, 1.0);
    }
  )";

  vertShader = Shader::compileSource(vert_source, GL_VERTEX_SHADER);
  fragShader = Shader::compileSource(frag_source, GL_FRAGMENT_SHADER);

  /* Link the shaders to a program */
  program = glCreateProgram();
  glAttachShader(program, vertShader);
  glAttachShader(program, fragShader);
  glLinkProgram(program);
}

TriangleProgram::~TriangleProgram() {
  glDeleteProgram(program);
  glDeleteShader(vertShader);
  glDeleteShader(fragShader);
  glDeleteBuffers(2, &vboHandles[0]);
  glDeleteVertexArrays(1, &vaoHandle);
}

void TriangleProgram::draw() {
  glUseProgram(program);
  glBindVertexArray(vaoHandle);
  /* draw points 0-3 from the currently bound VAO with current in-use shader
  */
  glDrawArrays(GL_TRIANGLES, 0, 3);
}
