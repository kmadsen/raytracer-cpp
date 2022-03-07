#include "RasterWindow.h"
#include "TriangleProgram.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

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

  GLFWwindow* window = glfwCreateWindow(640, 480, "Hello Triangle", NULL, NULL);
  if (!window) {
    fprintf(stderr, "ERROR: could not open window with GLFW3\n");
    glfwTerminate();
    exit(EXIT_FAILURE);
  }
  glfwMakeContextCurrent(window);

  /* start GLEW extension handler */
  glewInit();

  /* get version info */
  const GLubyte* renderer = glGetString(GL_RENDERER); /* get renderer string */
  const GLubyte* version = glGetString(GL_VERSION);   /* version as a string */
  printf("Renderer: %s\n", renderer);
  printf("OpenGL version supported %s\n", version);

  /*
   * tell GL to only draw onto a pixel if the shape is closer to the viewer
   * than anything already drawn at that pixel
   */
  glEnable(GL_DEPTH_TEST); /* enable depth-testing */
  /*
   * with LESS depth-testing interprets a smaller depth value as meaning
   * "closer"
   */
  glDepthFunc(GL_LESS);

  // Create a scope for the programs, they will destructor when the loop exits.
  {
    TriangleProgram triangleProgram = TriangleProgram();

    /*
    * This loop clears the drawing surface, then draws the geometry described
    * by the VAO onto the drawing surface. we 'poll events' to see if the window
    * was closed, etc. finally, we 'swap the buffers' which displays our drawing
    * surface onto the view area. we use a double-buffering system which means
    * that we have a 'currently displayed' surface, and 'currently being drawn'
    * surface. hence the 'swap' idea. in a single-buffering system we would see
    * stuff being drawn one-after-the-other
    */
    while (!glfwWindowShouldClose(window)) {
      /* wipe the drawing surface clear */
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      triangleProgram.draw();

      /* update other events like input handling */
      glfwPollEvents();
      /* put the stuff we've been drawing onto the display */
      glfwSwapBuffers(window);
    }
  }

  /* close GL context and any other GLFW resources */
  glfwDestroyWindow(window);
  glfwTerminate();
}
