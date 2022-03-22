#include "RasterWindow.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "CameraProgram.h"
#include "FpsConsoleLog.h"
#include "FrameBufferTexture.h"
#include "ScreenProgram.h"

using std::cout;
using std::endl;

RasterWindow::~RasterWindow() { delete frameBufferTexture; }

void RasterWindow::start(raster::Scene* scene) {
  glfwSetErrorCallback([](int error, const char* description) {
    fprintf(stderr, "Glfw error %s\n", description);
  });

  /* start GL context and O/S window using the GLFW helper library */
  if (!glfwInit()) {
    fprintf(stderr, "ERROR: could not start GLFW3\n");
    return;
  }

  /*
   * Version 4.6 Core is a good default that should run on just about
   * everything. Adjust later to suit project requirements.
   */
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  auto title = "Hello OpenGL";
  GLFWwindow* window =
      glfwCreateWindow(scene->width, scene->height, title, NULL, NULL);
  if (!window) {
    fprintf(stderr, "ERROR: could not open window with GLFW3\n");
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  glfwMakeContextCurrent(window);
  glfwSetWindowUserPointer(window, this);
  glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int w, int h) {
    static_cast<RasterWindow*>(glfwGetWindowUserPointer(window))->resize(w, h);
  });

  /* start GLEW extension handler */
  glewInit();

  /* get version info */
  const GLubyte* renderer = glGetString(GL_RENDERER);
  const GLubyte* version = glGetString(GL_VERSION);
  printf("Renderer: %s\n", renderer);
  printf("OpenGL version supported %s\n", version);

  // Create a scope for the programs, they will destructor when the loop exits.
  {
    frameBufferTexture = new FrameBufferTexture(scene->width, scene->height);
    auto cameraProgram = CameraProgram(scene);
    // auto screenProgram = ScreenProgram();

    // Start the render loop
    auto fpsLog = FpsConsoleLog();
    while (!glfwWindowShouldClose(window)) {
      // frameBufferTexture->bind();
      glEnable(GL_DEPTH_TEST);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      cameraProgram.draw();

      glBindFramebuffer(GL_FRAMEBUFFER, 0);

      // glDisable(GL_DEPTH_TEST);

      // glActiveTexture(GL_TEXTURE0);
      // glBindTexture(GL_TEXTURE_2D, frameBufferTexture->texture);
      // screenProgram.draw();

      // Swap buffers and poll input events
      glfwSwapBuffers(window);
      glfwPollEvents();
      fpsLog.frameEnd();
    }
  }

  /* close GL context and any other GLFW resources */
  glfwDestroyWindow(window);
  glfwTerminate();
}

void RasterWindow::resize(int width, int height) {
  glViewport(0, 0, width, height);
  frameBufferTexture->resize(width, height);
}
