/*
 *
 * Kyle Madsen - Simple Ray Tracing project
 *
 * The purpose of this file is to build background
 * classes to make Ray Tracing easier.
 *
 */
#include "RayTracerOpenGL.h"
#include "Parser.h"
#include "Scene.h"
#include "Image.h"
#include "LegacySceneGlut.h"

#include <time.h>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <thread>
#include <vector>

#define USING_OPENGL

SceneGlut *sceneGlut;

void idle(void) {
  sceneGlut->idle();
}

void keyboard(unsigned char key, int x, int y) {
  sceneGlut->keyboard(key, x, y);
}

void myGlutMouse(int button, int state, int x, int y) {
  sceneGlut->mousePress(button, state, x, y);
}

void myGlutMotion(int x, int y) {
  sceneGlut->mouseMotion(x, y);
}

// the window has changed shapes, fix ourselves up
void reshape(int x, int y) {
  sceneGlut->windowReshape(x, y);
}

void drawThread(void* args) {
  sceneGlut->drawThread(args);
}

void drawObjects(Vector lookdir) {
  sceneGlut->drawObjects(lookdir);
}

void updateScene() {
  sceneGlut->updateScene();
}

void display() {
  sceneGlut->display();
}

void RayTracerOpenGL::start(int argc, char** argv) {
  std::ifstream sceneFile(argv[1]);

  // parse the scene file
  std::string filename = argv[1];
  Parser parser(sceneFile);
  auto scene = parser.parseScene(filename);

  auto width = scene->getImage()->getXresolution();
  auto height = scene->getImage()->getYresolution();

  sceneGlut = new LegacySceneGlut();

  //
  // create the glut window
  //
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL);
  glutInitWindowSize(width, height);
  glutInitWindowPosition(100, 100);
  int main_window = glutCreateWindow("Ray Tracing");

  sceneGlut->init(main_window, scene);

  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);
  glutMouseFunc(myGlutMouse);
  glutMotionFunc(myGlutMotion);
  glutReshapeFunc(reshape);
  glutIdleFunc(idle);

  glEnable(GL_NORMALIZE);

  // give control over to glut
  glutMainLoop();

  std::cout << "Exit happened" << "\n";
}
