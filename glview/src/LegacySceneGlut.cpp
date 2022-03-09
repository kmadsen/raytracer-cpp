/*
 *
 * Kyle Madsen - Simple Ray Tracing project
 *
 * The purpose of this file is to build background
 * classes to make Ray Tracing easier.
 *
 */
#include "LegacySceneGlut.h"

#include "Color.h"
#include "ConstantBackground.h"
#include "GL/glut.h"
#include "Group.h"
#include "Image.h"
#include "Light.h"
#include "Object.h"
#include "Parser.h"
#include "PinholeCamera.h"
#include "Point.h"
#include "Ray.h"
#include "Scene.h"
#include "Vector.h"

#define USING_OPENGL

using std::cout;

void normalize(float v[3]) {
  float l = v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
  l = 1.0f / (float)sqrt(l);

  v[0] *= l;
  v[1] *= l;
  v[2] *= l;
}

void crossproduct(float a[3], float b[3], float res[3]) {
  res[0] = (a[1] * b[2] - a[2] * b[1]);
  res[1] = (a[2] * b[0] - a[0] * b[2]);
  res[2] = (a[0] * b[1] - a[1] * b[0]);
}

float length(float v[3]) {
  return (float)sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
}

void LegacySceneGlut::idle(void) {
  if (glutGetWindow() != main_window) glutSetWindow(main_window);

  // just keep redrawing the scene over and over
  glutPostRedisplay();
}

void LegacySceneGlut::keyboard(unsigned char key, int x, int y) {
  switch (key) {
    // quit
    case 27:
    case 'q':
    case 'Q':
      exit(0);
      break;
    case 't':
    case 'T':
      hybridOn = !hybridOn;
      break;
    case 'r':
    case 'R':
      multiThreadOn = !multiThreadOn;
  }
  glutPostRedisplay();
}

// catch mouse up/down events
void LegacySceneGlut::mousePress(int button, int state, int x, int y) {
  if (state == GLUT_DOWN)
    cur_button = button;
  else {
    if (button == cur_button) cur_button = -1;
  }

  last_x = x;
  last_y = y;
}

// catch mouse move events
void LegacySceneGlut::mouseMotion(int x, int y) {
  // the change in mouse position
  int dx = x - last_x;
  int dy = y - last_y;

  float scale, len, theta;
  float neye[3], neye2[3];
  float f[3], r[3], u[3];

  switch (cur_button) {
    case GLUT_LEFT_BUTTON:
      // translate
      f[0] = lookat[0] - eye[0];
      f[1] = lookat[1] - eye[1];
      f[2] = lookat[2] - eye[2];
      u[0] = 0;
      u[1] = 1;
      u[2] = 0;

      // scale the change by how far away we are
      scale = sqrt(length(f)) * 0.007;

      crossproduct(f, u, r);
      crossproduct(r, f, u);
      normalize(r);
      normalize(u);

      eye[0] += -r[0] * dx * scale + u[0] * dy * scale;
      eye[1] += -r[1] * dx * scale + u[1] * dy * scale;
      eye[2] += -r[2] * dx * scale + u[2] * dy * scale;

      lookat[0] += -r[0] * dx * scale + u[0] * dy * scale;
      lookat[1] += -r[1] * dx * scale + u[1] * dy * scale;
      lookat[2] += -r[2] * dx * scale + u[2] * dy * scale;

      break;

    case GLUT_MIDDLE_BUTTON:
      // zoom
      f[0] = lookat[0] - eye[0];
      f[1] = lookat[1] - eye[1];
      f[2] = lookat[2] - eye[2];

      len = length(f);
      normalize(f);

      // scale the change by how far away we are
      len -= sqrt(len) * dx * 0.03;

      eye[0] = lookat[0] - len * f[0];
      eye[1] = lookat[1] - len * f[1];
      eye[2] = lookat[2] - len * f[2];

      // make sure the eye and lookat points are sufficiently far away
      // push the lookat point forward if it is too close
      if (len < 1) {
        std::cout << "lookat move: " << len << "\n";
        lookat[0] = eye[0] + f[0];
        lookat[1] = eye[1] + f[1];
        lookat[2] = eye[2] + f[2];
      }

      break;

    case GLUT_RIGHT_BUTTON:
      // rotate

      neye[0] = eye[0] - lookat[0];
      neye[1] = eye[1] - lookat[1];
      neye[2] = eye[2] - lookat[2];

      // first rotate in the x/z plane
      theta = -dx * 0.007;
      neye2[0] = (float)cos(theta) * neye[0] + (float)sin(theta) * neye[2];
      neye2[1] = neye[1];
      neye2[2] = -(float)sin(theta) * neye[0] + (float)cos(theta) * neye[2];

      // now rotate vertically
      theta = -dy * 0.007;

      f[0] = -neye2[0];
      f[1] = -neye2[1];
      f[2] = -neye2[2];
      u[0] = 0;
      u[1] = 1;
      u[2] = 0;
      crossproduct(f, u, r);
      crossproduct(r, f, u);
      len = length(f);
      normalize(f);
      normalize(u);

      neye[0] = len * ((float)cos(theta) * f[0] + (float)sin(theta) * u[0]);
      neye[1] = len * ((float)cos(theta) * f[1] + (float)sin(theta) * u[1]);
      neye[2] = len * ((float)cos(theta) * f[2] + (float)sin(theta) * u[2]);

      eye[0] = lookat[0] - neye[0];
      eye[1] = lookat[1] - neye[1];
      eye[2] = lookat[2] - neye[2];

      break;
  }

  scene->getCamera()->setEye(Point(eye[0], eye[1], eye[2]));
  scene->getCamera()->setLookat(Point(lookat[0], lookat[1], lookat[2]));
  sceneIsDirty = true;

  last_x = x;
  last_y = y;

  glutPostRedisplay();
}

// the window has changed shapes, fix ourselves up
void LegacySceneGlut::windowReshape(int x, int y) {
  width = x;
  height = y;
  // printf("%d %d\n", x,y);
  glViewport(0, 0, width, height);

  scene->setImageResolution(width, height);

  // this will force all the values to update.
  sceneIsDirty = true;

  glutPostRedisplay();
}

void LegacySceneGlut::drawThread(void* args) {
  // Vector* vector = (Vector*)args;
  // scene->renderOpenGL(hybridOn, vector);

  RenderContext* context = (RenderContext*)args;
  Scene* s = context->getScene();
  s->renderRasterizedScene((*context));
}

void LegacySceneGlut::drawObjects(Vector lookdir) {
  scene->renderOpenGL(lookdir);

  if (hybridOn && !multiThreadOn) {
    RenderContext* context = new RenderContext(scene);
    int resX = scene->getImage()->getXresolution();
    int resY = scene->getImage()->getYresolution();
    context->setBounds(0, 0, resX, resY);
    scene->renderRasterizedScene((*context));
  }
}

void LegacySceneGlut::updateScene() {
  scene->preprocess();

  PinholeCamera* camera = (PinholeCamera*)scene->getCamera();
  Point sceneEye = camera->getEye();
  Point sceneLookat = camera->getLookat();
  up = camera->getUp();
  fov = camera->getFieldOfView();

  eye[0] = sceneEye.x;
  eye[1] = sceneEye.y;
  eye[2] = sceneEye.z;

  lookat[0] = sceneLookat.x;
  lookat[1] = sceneLookat.y;
  lookat[2] = sceneLookat.z;

  sceneIsDirty = false;
}

void LegacySceneGlut::display() {
  double start, end, dif;
  start = (double)clock();

  // clear the buffers
  Color background = scene->getBackground()->getBackground();
  glClearColor(background.r(), background.g(), background.b(), 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

  if (sceneIsDirty) {
    updateScene();
	}

  // projection transform
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(fov, width / height, .1, 100000);
  glMatrixMode(GL_MODELVIEW);

  // camera transform
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(eye[0], eye[1], eye[2], lookat[0], lookat[1], lookat[2], up.x, up.y, up.z);
  scene->setUpLightsOpenGL();

  // render the scene
  Vector lookdir(lookat[0] - eye[0], lookat[1] - eye[1], lookat[2] - eye[2]);
  lookdir.normalize();
  drawObjects(lookdir);

  // glDrawPixels(512, 512, GL_RGB, GL_FLOAT, scene->getPixels());

  // now display the image that was created.
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0, width, 0, height, -100, 10000);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

	cout << "hybrinOn: " << hybridOn << "\n";
	if (hybridOn) {
    glDisable(GL_LIGHTING);

    glBegin(GL_POINTS);
    Image* image = scene->getImage();
    int resX = image->getXresolution();
    int resY = image->getYresolution();
    for (int x = 0; x < resX; x++) {
      for (int y = 0; y < resY; y++) {
        Pixel p = image->get(x, y);
        // Set color
        glColor3f(p.r / 255.0f, p.g / 255.0f, p.b / 255.0f);

        // Set position
        glVertex3f(x, y, 0);
      }
    }
    glEnd();

    glEnable(GL_LIGHTING);
  }

  end = (double)clock();
  dif = (end - start) / CLOCKS_PER_SEC;
  printf("Total Render Time: %f seconds\n", dif);

  glutSwapBuffers();
}

