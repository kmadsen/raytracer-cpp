#ifndef SceneGlut_h
#define SceneGlut_h

#include "GL/glut.h"
#include "Vector.h"
#include "Scene.h"

class SceneGlut
{
public:
  SceneGlut() {}
  virtual ~SceneGlut() {}

  virtual void idle(void) = 0;
  virtual void keyboard(unsigned char key, int x, int y) = 0;
  virtual void mousePress(int button, int state, int x, int y) = 0;
  virtual void mouseMotion(int x, int y) = 0;
  virtual void windowReshape(int x, int y) = 0;
  virtual void drawThread(void* args) = 0;
  virtual void drawObjects(Vector lookdir) = 0;
  virtual void updateScene() = 0;
  virtual void display() = 0;
  
  virtual void init(int glut_window, Scene* scene) = 0;
};

#endif // SceneGlut_h
