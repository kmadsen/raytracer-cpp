#include "Shader.h"

#include <fstream>
#include <iostream>
#include <vector>

using std::ifstream;
using std::ios;
using std::vector;

GLuint Shader::compileFile(const char* fileName, const GLuint& shaderType) {
  GLuint shader = glCreateShader(shaderType);
  if (shader == 0) {
    fprintf(stderr, "Error creating shaderType: %d.\n", shaderType);
    exit(EXIT_FAILURE);
  }
  std::vector<GLchar> shaderCode;
  if (!Shader::loadFile(fileName, shaderCode)) {
    fprintf(stderr, "Error loading shader: %s\n", fileName);
    exit(EXIT_FAILURE);
  }
  const GLchar* codeArray[] = { &(shaderCode[0]) };
  glShaderSource(shader, 1, codeArray, NULL);
  glCompileShader(shader);

  // Check the compile status
  if (!Shader::check(shader)) {
    fprintf(stderr, "Error compiling shader.\n");
    exit(EXIT_FAILURE);
  }

  return shader;
}

GLuint Shader::compileSource(const char* source, const GLuint& shaderType) {
  GLuint shader = glCreateShader(shaderType);
  if (shader == 0) {
    fprintf(stderr, "Error creating shaderType: %d.\n", shaderType);
    exit(EXIT_FAILURE);
  }
  glShaderSource(shader, 1, &source, NULL);
  glCompileShader(shader);

  // Check the compile status
  if (!Shader::check(shader)) {
    fprintf(stderr, "Error compiling shader source:\n%s\n", source);
    exit(EXIT_FAILURE);
  }

  return shader;
}

bool Shader::check(const GLuint& shader) {
  GLint result;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
  if (GL_TRUE != result) {
    GLint logLen = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLen);
    if (logLen > 0) {
      char* log = new char[logLen];
      GLsizei written;
      glGetShaderInfoLog(shader, logLen, &written, log);
      fprintf(stderr, "Shader error log:\n%s\n", log);
      delete[] log;
    }
    return false;
  }
  return true;
}

bool Shader::loadFile(const char* fileName, vector<GLchar> shaderCode) {
  ifstream file;
  file.open(fileName, ios::in);
  if (file.is_open()) {
    file.seekg(0, ios::beg);
    int count = 0;
    while (file.good()) {
      int c = file.get();
      shaderCode.push_back(c);
      count++;
    }
    shaderCode[count - 1] = 0;
    file.close();
    return true;
  } else {
    return false;
  }
}

void Shader::printAttributes(GLuint programHandle) {
  GLint maxLength, nAttribs;
  glGetProgramiv(programHandle, GL_ACTIVE_ATTRIBUTES, &nAttribs);
  glGetProgramiv(programHandle, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxLength);

  GLchar* name = new GLchar[maxLength];

  GLint written, size, location;
  GLenum type;
  printf(" Index | Name\n");
  printf("-----------------------------\n");
  for (int i = 0; i < nAttribs; i++) {
    glGetActiveAttrib(programHandle, i, maxLength, &written, &size, &type,
                      name);
    location = glGetAttribLocation(programHandle, name);
    printf(" %-5d | %s\n", location, name);
  }

  delete[] name;
}

void Shader::printActiveUniformVars(GLuint programHandle) {
  GLint nUniforms, maxLength;
  glGetProgramiv(programHandle, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxLength);
  glGetProgramiv(programHandle, GL_ACTIVE_UNIFORMS, &nUniforms);

  GLchar* name = new GLchar[maxLength];

  GLint size, location;
  GLsizei written;
  GLenum type;
  printf(" Location | Name\n");
  printf("-----------------------------\n");
  for (int i = 0; i < nUniforms; i++) {
    glGetActiveUniform(programHandle, i, maxLength, &written, &size, &type,
                       name);
    location = glGetUniformLocation(programHandle, name);
    printf(" %-8d | %s\n", location, name);
  }

  delete[] name;
}
