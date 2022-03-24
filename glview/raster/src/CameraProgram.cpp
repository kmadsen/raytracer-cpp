#include "CameraProgram.h"

#include <GL/glew.h>
#include <stdio.h>
#include <sstream>

#include <glm/geometric.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "RasterScene.h"
#include "Shader.h"
#include "TriangleBuffer.h"

CameraProgram::CameraProgram(raster::Scene* _scene) {
  scene = _scene;

  triangleBuffer = new TriangleBuffer(scene->triangles);
  triangleBuffer->bind();

  glGenBuffers(1, &uboViewProjectionHandle);

  /**
   * Bind a uniform buffer with projection and view matrices
   */
  glBindBuffer(GL_UNIFORM_BUFFER, uboViewProjectionHandle);
  glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
  glBindBuffer(GL_UNIFORM_BUFFER, 0);
  glBindBufferRange(GL_UNIFORM_BUFFER, 0, uboViewProjectionHandle, 0,
                    2 * sizeof(glm::mat4));

  // Bind the projection matrix to a uniform buffer
  float fieldOfView = scene->camera->fieldOfView;
  float aspectRatio = scene->aspectRatio();
  glm::mat4 projection =
      glm::perspective(glm::radians(fieldOfView), aspectRatio, 0.1f, 100.0f);

  glBindBuffer(GL_UNIFORM_BUFFER, uboViewProjectionHandle);
  glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4),
                  glm::value_ptr(projection));
  glBindBuffer(GL_UNIFORM_BUFFER, 0);

  // Bind the view matrix to the uniform buffer
  glm::mat4 view = scene->camera->view;
  glBindBuffer(GL_UNIFORM_BUFFER, uboViewProjectionHandle);
  glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4),
                  glm::value_ptr(view));
  glBindBuffer(GL_UNIFORM_BUFFER, 0);

  /*
   * These are the strings of code for the shaders
   * the vertex shader positions each vertex point
   */
  const char* vert_source = R"(
    #version 460

    layout (location=0) in vec3 aPosition;
    layout (location=1) in vec3 aNormal;
    layout (location=2) in vec3 aColor;
    layout (location=3) in float aAmbient;
    layout (location=4) in float aDiffuse;

    layout (std140, binding = 0) uniform Matrices {
      mat4 projection;
      mat4 view;
    };
    uniform mat4 model;

    layout (location=0) out vec3 vPosition;
    layout (location=1) out vec3 vNormal;
    layout (location=2) out vec3 vColor;
    layout (location=3) out float vAmbient;
    layout (location=4) out float vDiffuse;

    void main () {
      vColor = aColor;
      vAmbient = aAmbient;
      vDiffuse = aDiffuse;

      vPosition = vec3(model * vec4(aPosition, 1.0));
      mat4 mvpMatrix = projection * view * model;
      vNormal = mat3(transpose(inverse(model))) * aNormal;
      gl_Position = mvpMatrix * vec4(aPosition, 1.0);
    };
  )";

  if (scene->pointLights.size() > 3) {
    fprintf(
        stderr,
        "The fragment shader source does not allow more than 2 point lights\n");
    fprintf(stderr, "  you gave %d point lights\n", scene->pointLights.size());
    exit(EXIT_FAILURE);
  }

  /*
   * The fragment shader colours each fragment (pixel-sized area of the
   * triangle)
   */
  const char* frag_source = R"(
    #version 460
    #define MAX_POINT_LIGHTS 2

    layout (location=0) in vec3 vPosition;
    layout (location=1) in vec3 vNormal;
    layout (location=2) in vec3 vColor;
    layout (location=3) in float vAmbient;
    layout (location=4) in float vDiffuse;

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
      vec3 allLightsColor = vec3(sceneAmbient * vAmbient);

      // diffuse
      vec3 normal = normalize(vNormal);
      for (int i = 0; i < numPointLights; i++) {
        PointLight pointLight = pointLights[i];
        vec3 lightDir = normalize(pointLight.position - vPosition);
        float cosphi = dot(normal, lightDir);

        vec3 diffuse = pointLight.color * (cosphi * vec3(1.0));
        allLightsColor += diffuse * vDiffuse;
      }

      fragmentColor = vec4(allLightsColor * vColor, 1.0);
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
  glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_FALSE,
                     &model[0][0]);

  // Lighting info
  int numPointLights = scene->pointLights.size();
  glUniform1i(glGetUniformLocation(program, "numPointLights"), numPointLights);
  for (int i = 0; i < numPointLights; i++) {
    std::stringstream ssPosition;
    ssPosition << "pointLights[" << i << "].position";
    glUniform3fv(glGetUniformLocation(program, ssPosition.str().c_str()), 1,
                 &scene->pointLights[i]->position[0]);
    std::stringstream ssColor;
    ssColor << "pointLights[" << i << "].color";
    glUniform3fv(glGetUniformLocation(program, ssColor.str().c_str()), 1,
                 &scene->pointLights[i]->color[0]);
  }
  glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_FALSE,
                     &model[0][0]);

  Shader::printActiveUniformVars(program);
  Shader::printAttributes(program);
}

CameraProgram::~CameraProgram() {
  glDeleteProgram(program);
  glDeleteShader(vertShader);
  glDeleteShader(fragShader);
  delete triangleBuffer;
  glDeleteBuffers(1, &uboViewProjectionHandle);
}

void CameraProgram::draw() {
  glUseProgram(program);
  triangleBuffer->draw();
}
