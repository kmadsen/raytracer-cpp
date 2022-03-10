#include "RasterWindow.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "FpsConsoleLog.h"
#include "FrameBufferTexture.h"
#include "RectangleProgram.h"
#include "ScreenProgram.h"
#include "TriangleProgram.h"

using std::cout;
using std::endl;

RasterWindow::~RasterWindow() { delete frameBufferTexture; }

void RasterWindow::start(int argc, char** argv) {
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

  int width = 800;
  int height = 600;
  auto title = "Hello OpenGL";
  GLFWwindow* window = glfwCreateWindow(width, height, title, NULL, NULL);
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
  const GLubyte* renderer = glGetString(GL_RENDERER); /* get renderer string */
  const GLubyte* version = glGetString(GL_VERSION);   /* version as a string */
  printf("Renderer: %s\n", renderer);
  printf("OpenGL version supported %s\n", version);

  // Create a scope for the programs, they will destructor when the loop exits.
  {
    frameBufferTexture = new FrameBufferTexture(width, height);
    auto rectangleProgram = RectangleProgram();
    auto triangleProgram = TriangleProgram();
    auto screenProgram = ScreenProgram();

    /*
     * This loop clears the drawing surface, then draws the geometry described
     * by the VAO onto the drawing surface. we 'poll events' to see if the
     * window was closed, etc. finally, we 'swap the buffers' which displays our
     * drawing surface onto the view area. we use a double-buffering system
     * which means that we have a 'currently displayed' surface, and 'currently
     * being drawn' surface. hence the 'swap' idea. in a single-buffering system
     * we would see stuff being drawn one-after-the-other
     */
    auto fpsLog = FpsConsoleLog();
    while (!glfwWindowShouldClose(window)) {
      frameBufferTexture->bind();
      glEnable(GL_DEPTH_TEST);
      glDepthFunc(GL_LESS);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      glDisable(GL_BLEND);
      triangleProgram.draw();

      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC1_ALPHA);
      rectangleProgram.draw();

      glBindFramebuffer(GL_FRAMEBUFFER, 0);

      glDisable(GL_DEPTH_TEST);
      glDisable(GL_BLEND);

      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, frameBufferTexture->texture);
      screenProgram.draw();

      /* update other events like input handling */
      /* put the stuff we've been drawing onto the display */
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
