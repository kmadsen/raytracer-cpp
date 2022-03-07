#include "ScreenProgram.h"

#include <GLFW/glfw3.h>
#include <stdio.h>

#include "Shader.h"

ScreenProgram::ScreenProgram() {
  // Position--|-Colors-----------|-Texture----
  GLfloat vertices[] = {
    -1.f,  1.f,  1.0f, 0.0f, 0.0f,  0.0f, 1.0f,  // top left
     1.f,  1.f,  0.0f, 1.0f, 0.0f,  1.0f, 1.0f,  // top right
     1.f, -1.f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f,  // bottom right
    -1.f, -1.f,  1.0f, 0.0f, 1.0f,  0.0f, 0.0f   // bottom left
  };
  GLuint indices[] = {
    0, 1, 2,
    2, 3, 0
  };

  glGenVertexArrays(1, &vaoHandle);
  glGenBuffers(1, &vboHandle);
  glGenBuffers(1, &eboHandle);

  glBindVertexArray(vaoHandle);
  glBindBuffer(GL_ARRAY_BUFFER, vboHandle);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboHandle);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);

  // Position attribute
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), 0);
  glEnableVertexAttribArray(0);
  // Color attribute
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat),
                        reinterpret_cast<void*>(2 * sizeof(GLfloat)));
  glEnableVertexAttribArray(1);
  // Texture coorindate attribute
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat),
                        reinterpret_cast<void*>(5 * sizeof(GLfloat)));
  glEnableVertexAttribArray(2);  // TexCoords
  glBindVertexArray(0);

  /*
   * These are the strings of code for the shaders
   * the vertex shader positions each vertex point
   */
  const char* vert_source = R"(
    #version 460

    layout (location=0) in vec2 aPosition;
    layout (location=1) in vec3 aColor;
    layout (location=2) in vec2 aTexCoord;

    layout (location=0) out vec3 vColor;
    layout (location=1) out vec2 vTexture;

    void main () {
      gl_Position = vec4(aPosition, 0.0, 1.0);
      vColor = aColor;
      vTexture = aTexCoord;
    };
  )";

  /*
   * The fragment shader colours each fragment (pixel-sized area of the
   * triangle)
   */
  const char* frag_source = R"(
    #version 460

    layout (location=0) out vec4 fragmentColor;

    layout (location=0) in vec3 vColor;
    layout (location=1) in vec2 vTexture;

    uniform sampler2D uTexture;

    void main () {
      fragmentColor = texture(uTexture, vTexture);
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

ScreenProgram::~ScreenProgram() {
  glDeleteProgram(program);
  glDeleteShader(vertShader);
  glDeleteShader(fragShader);
  glDeleteBuffers(1, &vboHandle);
  glDeleteBuffers(1, &eboHandle);
  glDeleteVertexArrays(1, &vaoHandle);
  glDeleteFramebuffers(1, &frameBuffer);
}

void ScreenProgram::draw() {
  glUseProgram(program);
  glBindVertexArray(vaoHandle);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboHandle);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
