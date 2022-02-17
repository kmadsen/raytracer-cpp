#ifndef LegacySceneGlut_h
#define LegacySceneGlut_h

#include "SceneGlut.h"
#include "Scene.h"
#include "Vector.h"

class LegacySceneGlut : public SceneGlut
{
public:
	LegacySceneGlut() {}
	virtual ~LegacySceneGlut(void) {}

  void idle(void);
  void keyboard(unsigned char key, int x, int y);
  void mousePress(int button, int state, int x, int y);
  void mouseMotion(int x, int y);
  void windowReshape(int x, int y);
  void drawThread(void* args);
  void drawObjects(Vector lookdir);
  void updateScene();
  void display();

	virtual void init(int glut_window, Scene* scene) {
		this->main_window = glut_window;
		this->scene = scene;
	}

protected:

	int main_window = 1;
	int width;
	int height;

	// if this is true then hybrid algorithms are used.
	bool hybridOn = true;
	bool multiThreadOn = false;
	bool sceneIsDirty = true;

	Scene* scene;
	float eye[3];
	float lookat[3];
	Vector up;
	double fov;

	// Mouse gestures
	int cur_button = -1;
	int last_x;
	int last_y;

};

#endif // LegacySceneGlut
