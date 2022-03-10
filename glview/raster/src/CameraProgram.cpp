#include "CameraProgram.h"
#include "Shader.h"
#include "RasterScene.h"

#include <GL/glew.h>
#include <stdio.h>
#include <sstream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

CameraProgram::CameraProgram(raster::Scene* _scene) {
  scene = _scene;

  // Position---------|-Colors-----------|-Normal----
  GLfloat vertices[] = {
    1.0f,  1.0f, 4.0f,  1.0f, 0.0f, 0.0f,  -0.218218f, -0.872872, 0.436436f,  // top left
    3.0f, -0.5f, 2.0f,  0.0f, 1.0f, 0.0f,  -0.218218f, -0.872872, 0.436436f,  // top right
    3.0f,  1.5f, 6.0f,  0.0f, 1.0f, 1.0f,  -0.218218f, -0.872872, 0.436436f   // bottom right
  };

  GLuint indices[] = {
    0, 1, 2
  };

  glGenVertexArrays(1, &vaoHandle);
  glGenBuffers(1, &vboHandle);
  glGenBuffers(1, &eboHandle);
  glGenBuffers(1, &uboViewProjectionHandle);

  glBindVertexArray(vaoHandle);
  glBindBuffer(GL_ARRAY_BUFFER, vboHandle);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboHandle);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);

  // Position attribute
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), 0);
  // Color attribute
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), reinterpret_cast<void*>(3 * sizeof(GLfloat)));
  // Normal attribute
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), reinterpret_cast<void*>(7 * sizeof(GLfloat)));
  glBindVertexArray(0);

  /**
   * Bind a uniform buffer with projection and view matrices
   */
  glBindBuffer(GL_UNIFORM_BUFFER, uboViewProjectionHandle);
  glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
  glBindBuffer(GL_UNIFORM_BUFFER, 0);

  glBindBufferRange(GL_UNIFORM_BUFFER, 0, uboViewProjectionHandle, 0, 2 * sizeof(glm::mat4));

  // Bind the projection matrix to a uniform buffer
  float aspectRatio = scene->aspectRatio();
  glm::mat4 projection = glm::perspective(glm::radians(26.0f), aspectRatio, 0.1f, 100.0f);
  glBindBuffer(GL_UNIFORM_BUFFER, uboViewProjectionHandle);
  glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(projection));
  glBindBuffer(GL_UNIFORM_BUFFER, 0);

  // Bind the view matrix to the uniform buffer
  glm::mat4 view = scene->camera()->view;
  glBindBuffer(GL_UNIFORM_BUFFER, uboViewProjectionHandle);
  glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view));
  glBindBuffer(GL_UNIFORM_BUFFER, 0);

  /*
   * These are the strings of code for the shaders
   * the vertex shader positions each vertex point
   */
  const char* vert_source = R"(
    #version 460

    layout (location=0) in vec3 aPosition;
    layout (location=1) in vec3 aColor;
    layout (location=2) in vec3 aNormal;

    layout (std140, binding = 0) uniform Matrices {
      mat4 projection;
      mat4 view;
    };
    uniform mat4 model;

    layout (location=0) out vec3 vColor;
    layout (location=1) out vec3 vNormal;
    layout (location=2) out vec3 vPosition;

    void main () {
      vColor = aColor;
      vPosition = vec3(model * vec4(aPosition, 1.0));
      mat4 mvpMatrix = projection * view * model;
      vNormal = mat3(transpose(inverse(model))) * aNormal;
      gl_Position = mvpMatrix * vec4(aPosition, 1.0);
    };
  )";

  if (scene->pointLights().size() > 3) {
    fprintf(stderr, "The fragment shader source does not allow more than 2 point lights\n");
    fprintf(stderr, "  you gave %d point lights\n", scene->pointLights().size());
    exit(EXIT_FAILURE);
  }

  /*
   * The fragment shader colours each fragment (pixel-sized area of the
   * triangle)
   */
  const char* frag_source = R"(
    #version 460
    #define MAX_POINT_LIGHTS 2

    layout (location=0) in vec3 vColor;
    layout (location=1) in vec3 vNormal;
    layout (location=2) in vec3 vPosition;

    layout (std140, binding = 0) uniform Matrices {
      mat4 projection;
      mat4 view;
    };
    uniform mat4 model;

    struct PointLight {
      vec3 position;
      vec3 color;
    };
    uniform int numPointLights;
    uniform PointLight pointLights[MAX_POINT_LIGHTS];

    layout (location=0) out vec4 fragmentColor;

    void main () {
      float sceneAmbient = 0.4;
      float materialAmbient = 0.4;
      vec3 materialColor = vec3(1.0, 1.0, 0.0);
      vec3 allLightsColor = vec3(sceneAmbient * materialAmbient);

      // diffuse
      vec3 normal = normalize(vNormal);
      for (int i = 0; i < numPointLights; i++) {
        PointLight pointLight = pointLights[i];
        vec3 lightDir = normalize(pointLight.position - vPosition);
        float diff = dot(normal, lightDir);
        vec3 diffuse = pointLight.color * (diff * vec3(1.0));
        allLightsColor += diffuse * 0.6;
      }

      fragmentColor = vec4(allLightsColor * materialColor, 1.0);
    }
  )";

  /* Link the shaders to a program */
  program = glCreateProgram();
  vertShader = Shader::compileSource(vert_source, GL_VERTEX_SHADER);
  fragShader = Shader::compileSource(frag_source, GL_FRAGMENT_SHADER);
  glAttachShader(program, vertShader);
  glAttachShader(program, fragShader);
  glLinkProgram(program);

  glUseProgram(program);

  // Model matrix
  glm::mat4 model(1.0f);
  glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_FALSE, &model[0][0]);

  // Lighting info
  int numPointLights = scene->pointLights().size();
  glUniform1i(glGetUniformLocation(program, "numPointLights"), numPointLights);
  for (int i = 0; i < numPointLights; i++) {
    std::stringstream ssPosition;
    ssPosition << "pointLights[" << i << "].position";
    glUniform3fv(glGetUniformLocation(program, ssPosition.str().c_str()), 1, &scene->pointLights()[i]->position[0]);
    std::stringstream ssColor;
    ssColor << "pointLights[" << i << "].color";
    glUniform3fv(glGetUniformLocation(program, ssColor.str().c_str()), 1, &scene->pointLights()[i]->color[0]);
  }
  glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_FALSE, &model[0][0]);

  Shader::printActiveUniformVars(program);
  Shader::printAttributes(program);
}

CameraProgram::~CameraProgram() {
  glDeleteProgram(program);
  glDeleteShader(vertShader);
  glDeleteShader(fragShader);
  glDeleteBuffers(1, &vboHandle);
  glDeleteBuffers(1, &eboHandle);
  glDeleteBuffers(1, &uboViewProjectionHandle);
  glDeleteVertexArrays(1, &vaoHandle);
}

void CameraProgram::draw() {
  glUseProgram(program);
  glBindVertexArray(vaoHandle);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboHandle);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
