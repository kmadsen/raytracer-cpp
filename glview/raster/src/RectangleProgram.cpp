#include "RectangleProgram.h"
#include "Shader.h"

#include <GL/glew.h>
#include <stdio.h>

RectangleProgram::RectangleProgram() {
  GLfloat vertices[] = {
    -0.9f,  0.3f, 0.0f,  // top left
     0.9f,  0.3f, 0.0f,  // top right
     0.9f, -0.9f, 0.0f,  // bottom right

     0.9f, -0.9f, 0.0f,  // bottom right
    -0.9f, -0.9f, 0.0f,  // bottom left
    -0.9f,  0.3f, 0.0f   // top left
  };
  GLfloat colors[] = {
    1.0f, 0.0f, 0.0f, 0.5,  // top left
    0.0f, 1.0f, 0.0f, 0.5,  // top right
    0.0f, 0.0f, 1.0f, 0.5,  // bottom right

    0.0f, 0.0f, 1.0f, 0.5,  // bottom right
    0.0f, 1.0f, 1.0f, 0.5,  // bottom left
    1.0f, 0.0f, 0.0f, 0.5   // top left
  };

  // Vertex buffer object (VBO). Generate buffers handle's to pass point and
  // color data to the vertex shader.
  glGenBuffers(2, vboHandles);
  GLuint pointsHandle = vboHandles[0];
  GLuint colorsHandle = vboHandles[1];

  // Bind the points and colors to the handles.
  glBindBuffer(GL_ARRAY_BUFFER, pointsHandle);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, colorsHandle);
  glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

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
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, NULL);
  glBindVertexArray(0);

  /*
  * These are the strings of code for the shaders
  * the vertex shader positions each vertex point
  */
  static const char* vert_source = R"(
    #version 460

    layout (location=0) in vec3 vertexPosition;
    layout (location=1) in vec4 vertexColor;

    layout (location=0) out vec4 vColor;

    void main () {
      vColor = vertexColor;
      gl_Position = vec4(vertexPosition, 1.0);
    };
  )";

  /*
  * The fragment shader colours each fragment (pixel-sized area of the triangle)
  */
  const char* frag_source = R"(
    #version 460

    layout (location=0) in vec4 vColor;
    layout (location=0) out vec4 fragmentColor;

    void main () {
      fragmentColor = vec4(vColor);
    }
  )";

  /* Link the shaders to a program */
  program = glCreateProgram();
  vertShader = Shader::compileSource(vert_source, GL_VERTEX_SHADER);
  fragShader = Shader::compileSource(frag_source, GL_FRAGMENT_SHADER);
  glAttachShader(program, vertShader);
  glAttachShader(program, fragShader);
  glLinkProgram(program);
}

RectangleProgram::~RectangleProgram() {
  glDeleteProgram(program);
  glDeleteShader(vertShader);
  glDeleteShader(fragShader);
  glDeleteBuffers(2, &vboHandles[0]);
  glDeleteVertexArrays(1, &vaoHandle);
  glDeleteFramebuffers(1, &frameBuffer);
}

void RectangleProgram::draw() {
  glUseProgram(program);
  glBindVertexArray(vaoHandle);
  glDrawArrays(GL_TRIANGLES, 0, 6);
}
