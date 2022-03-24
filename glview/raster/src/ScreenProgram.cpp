#include "ScreenProgram.h"

#include <GL/glew.h>
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
   * Use this program to blur the scene
   */
  const char* frag_source_blur = R"(
    #version 460

    layout (location=0) out vec4 fragmentColor;

    layout (location=0) in vec3 vColor;
    layout (location=1) in vec2 vTexture;

    uniform sampler2D uTexture;

    void main () {
      ivec2 textureSize2d = textureSize(uTexture, 0);
      vec2 blur = 2.0 / textureSize2d;

      vec4 sum = vec4(0.0);
      for (int x = -2; x <= 2; x++) {
        for (int y = -2; y <= 2; y++) {
          sum += texture(
            uTexture,
            vTexture + vec2(x, y) * blur
          );
        }
      }
      fragmentColor = sum / 25.0;
    }
  )";

  /*
   * Use this program for verifying basic rendering works.
   */
  const char* frag_source_noop = R"(
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
  fragShader = Shader::compileSource(frag_source_noop, GL_FRAGMENT_SHADER);
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
}

void ScreenProgram::draw() {
  glUseProgram(program);
  glBindVertexArray(vaoHandle);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboHandle);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
