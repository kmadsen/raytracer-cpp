#include "RasterWindow.h"
#include "Shader.h"

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
  GLuint vboHandles[2];
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
  GLuint vaoHandle;
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

  GLuint vert_shader = Shader::compileSource(vert_source, GL_VERTEX_SHADER);
  GLuint frag_shader = Shader::compileSource(frag_source, GL_FRAGMENT_SHADER);

  /* Link the shaders to a program */
  GLuint program = glCreateProgram();
  glAttachShader(program, frag_shader);
  glAttachShader(program, vert_shader);
  glLinkProgram(program);

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
    glUseProgram(program);
    glBindVertexArray(vaoHandle);
    /* draw points 0-3 from the currently bound VAO with current in-use shader
     */
    glDrawArrays(GL_TRIANGLES, 0, 3);
    /* update other events like input handling */
    glfwPollEvents();
    /* put the stuff we've been drawing onto the display */
    glfwSwapBuffers(window);
  }

  glDeleteProgram(program);
  glDeleteShader(vert_shader);
  glDeleteShader(frag_shader);
  glDeleteBuffers(2, &vboHandles[0]);
  glDeleteVertexArrays(1, &vaoHandle);

  /* close GL context and any other GLFW resources */
  glfwDestroyWindow(window);
  glfwTerminate();
}
