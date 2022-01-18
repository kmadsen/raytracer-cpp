/*
 *
 * Kyle Madsen - Simple Ray Tracing project
 *
 * The purpose of this file is to build background 
 * classes to make Ray Tracing easier.
 *
 */

#include "Scene.h"
#include "Vector.h"
#include "Parser.h"
#include "Image.h"
#include "Camera.h"
#include "Color.h"
#include "Ray.h"
#include "HitRecord.h"
#include "RenderContext.h"

#include "GL/glut.h"
#include "GL/glui.h"

#include <math.h>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <float.h>
using namespace std;

int	main_window;

Scene* scene;
bool canRender = false;

// pointers for all of the glui controls
GLUI *glui;
GLUI_Listbox *scene_listbox;
int listbox_item_id;

// the user id's that we can use to identify which control
// caused the callback to be called
#define CB_SCENE 0
#define CB_RENDER_BUTTON 1

int width = 0;
int height = 0;
int offsetX = 0;

////////////
// helper draw functions
////////////
void drawPoint(int x, int y, Pixel p)
{
	float r,g,b;
	r = (float)p.r/255.0f;
	g = (float)p.g/255.0f;
	b = (float)p.b/255.0f;
    //Set color
    glColor3f(r,g,b);
    //Set position
    glVertex3f(x, y, 0);
}

void drawPoint(int x, int y, Color c)
{
	glColor3f(c.r(), c.g(), c.b());
	glVertex3f(x - offsetX, height - y, 0);
}


void myGlutIdle(void)
{
	// make sure the main window is active
	if (glutGetWindow() != main_window)
		glutSetWindow(main_window);

	// if you have moving objects, you can do that here

	// just keep redrawing the scene over and over
	glutPostRedisplay();
}


int last_x = 10;
int last_y = 10;
Color mouseRayColor;

// catch mouse up/down events
void myGlutMouse(int button, int state, int x, int y)
{
	last_x = x;
	last_y = y;
	
	if (scene == 0x0)
		return;

	Ray ray;
	RenderContext context(scene);

	Color atten(1,1,1);
	Color result;

	int rayX = x - offsetX;
	int rayY = height - y;

	scene->getCamera()->makeRay(ray, context, rayX, rayY);
	HitRecord hitRec(DBL_MAX);
	scene->traceRay(result, context, ray, hitRec, atten, 0);

	mouseRayColor = result;
}

// catch mouse move events
void myGlutMotion(int x, int y)
{
	// the change in mouse position
	int dx = x-last_x;
	int dy = y-last_y;

	last_x = x;
	last_y = y;

	glutPostRedisplay();
}

// you can put keyboard shortcuts in here
void myGlutKeyboard(unsigned char key, int x, int y)
{
	switch(key)
	{
	// quit
	case 27: 
	case 'q':
	case 'Q':
		exit(0);
		break;
	}

	glutPostRedisplay();
}

// the window has changed shapes, fix ourselves up
void myGlutReshape(int	x, int y)
{
	int tx, ty, tw, th;
	GLUI_Master.get_viewport_area(&tx, &ty, &tw, &th);

	width = tw;
	height = th;
	offsetX = tx;
//	glViewport(tx, ty, tw, th);

//	glutPostRedisplay();

	glViewport(tx,ty,tw,th);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0,tw,0,th,-100,10000);
	glMatrixMode(GL_MODELVIEW);
    glutPostRedisplay();
}

// draw the scene
void myGlutDisplay(	void )
{
	glMatrixMode(GL_MODELVIEW);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    
    //Clear background to black 
	glClearColor(0,0,0,0);
	glLoadIdentity();
    
	glBegin(GL_POINTS);
	drawPoint(last_x, last_y, mouseRayColor);
	glEnd();

	if (canRender)
	{
		Image* image = scene->getImage();

		Pixel p;
		int xres = image->getXresolution();
		int yres = image->getYresolution();
		glBegin(GL_POINTS);
		for (int x = 0; x < image->getXresolution(); x ++) {
			for (int y = 0; y < image->getYresolution(); y++) {
				drawPoint(x,y, image->get(x,y));
			}
		}
		glEnd();

	}
	glutSwapBuffers(); 
}

void ParseSceneFile()
{
	GLUI_String filename = scene_listbox->curr_text;
	ifstream sceneFile(filename.c_str());
	if ( !sceneFile.good() ) {
		cerr << "This scene file was bad: " << endl;
		exit(1);
	}

	// Create a scene from the file input
	Parser parser(sceneFile);
	scene = parser.parseScene(filename);
}

void RenderScene()
{
	if (scene == 0x0)
		return;

	scene->preprocess();

	// TODO should set up some timer
	scene->renderScene();
	
	scene->getImage()->write("image.ppm");

	canRender = true;
}

// some controls generate a callback when they are changed
void glui_cb(int control)
{
	switch(control)
	{
	case CB_SCENE:
		ParseSceneFile();
		break;
	case CB_RENDER_BUTTON:
		ParseSceneFile();
		RenderScene();
		break;
	}

	glutPostRedisplay();
}



// entry point
int main(int argc,	char* argv[])
{
	//
	// create the glut window
	//
	glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE|GLUT_DEPTH);
	glutInitWindowSize(1000, 600);
	glutInitWindowPosition(100,100);
	main_window = glutCreateWindow("Sample Interface");

	//
	// set callbacks
	//
	glutDisplayFunc(myGlutDisplay);
	GLUI_Master.set_glutReshapeFunc(myGlutReshape);
	GLUI_Master.set_glutIdleFunc(myGlutIdle);
	GLUI_Master.set_glutKeyboardFunc(myGlutKeyboard);
	GLUI_Master.set_glutMouseFunc(myGlutMouse);
	glutMotionFunc(myGlutMotion);

	//
	// create the interface subwindow and add widgets
	//
	glui = GLUI_Master.create_glui_subwindow(main_window, GLUI_SUBWINDOW_LEFT);

	//  Add the Color listbox to the 'Object Properties' Panel
	scene_listbox = glui->add_listbox("Scene", &listbox_item_id, CB_SCENE, glui_cb);

	//  When you add a scene file to the project you should add the scene file
	//  here so you can select it in the list box.
	scene_listbox->add_item(1, "prog01.scn");
	scene_listbox->add_item(2, "prog02.scn");
	scene_listbox->add_item(3, "prog02_creative.scn");


	glui->add_button("Render", CB_RENDER_BUTTON, glui_cb);

	glui->set_main_gfx_window(main_window);

	// give control over to glut
	glutMainLoop();

	return 0;
}

/* Some sample thread code when you want to add it..

unsigned __stdcall renderScene(void* a) {
	// We are supposed to just run the Bus on port 2000.
	//Bus b(100, 2000);
	//Command::message("Starting Bus");
	//b.run();
	//Command::message("Stopping Bus");
	return 0;
}

		unsigned retBus, retSub, retPub;
		HANDLE busThread;
		busThread = (HANDLE)_beginthreadex(0, 0, doBus, (void*)NULL, 0, &retBus); // Start bus listening
		Sleep(2000); // Let the Bus get started
		HANDLE subThread = (HANDLE)_beginthreadex(0, 0, doSubscriber, (void*)psSub, 0, &retSub); // Start subscriber
		Sleep(2000); // Let the subscriptions happen before we fire off publishes.
		HANDLE pubThread = (HANDLE)_beginthreadex(0, 0, doPublisher, (void*)psPub, 0, &retPub); // Start publisher
		// Need to wait until bus exits.
		WaitForSingleObject(busThread, INFINITE); // Could use waitformultipleobjects to wait for all thraeds.
		WaitForSingleObject(subThread, INFINITE);
		WaitForSingleObject(pubThread, INFINITE);
		delete psSub;
		delete psPub;
		CloseHandle(busThread);
*/