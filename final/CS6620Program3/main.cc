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
#include "Group.h"
#include "PinholeCamera.h"
#include "Point.h"
#include "Vector.h"

#include "GL/glut.h"

#include <iomanip>
#include <iostream>
#include <fstream>
#include <time.h>
#include <vector>

#define USING_OPENGL

using namespace std;

int main_window = 1;
int width = 512;
int height = 512;

// if this is true then hybrid algorithms are used.
bool hybridOn = false;
bool sceneIsDirty = true;

Scene* scene;
Point eye;
Point lookat;
Vector up;

void idle(void)
{
	if (glutGetWindow() != main_window)
		glutSetWindow(main_window);

	// just keep redrawing the scene over and over
	glutPostRedisplay();
}

void toggleRenderMode()
{
	hybridOn = !hybridOn;
}

void keyboard(unsigned char key, int x, int y)
{
	switch(key)
	{
	// quit
	case 27: 
	case 'q':
	case 'Q':
		exit(0);
		break;
	case 't':
	case 'T':
		toggleRenderMode();
		break;
    }
    glutPostRedisplay();
}

// the window has changed shapes, fix ourselves up
void reshape(int x, int y)
{
	width = x;
	height = y;
	printf("%d %d\n", x,y);
	glViewport(0,0,width,height);
	
	// this will force all the values to update.
	sceneIsDirty = true;

	glutPostRedisplay();
}

void drawPoint(int x, int y, float r, float g, float b)
{
    //Set color
    glColor3f(r,g,b);
    
    //Set position
    glVertex3f(x,y,0);
}

void rayTraceScene()
{
	double start,end, dif;

	// preprocess the scene
	start = (double)clock();
	  scene->preprocess();
	end = (double)clock();
	dif = (end - start)/ CLOCKS_PER_SEC;
	printf ("Preprocessing time: %f seconds\n", dif);

	// render the scene
	start = (double)clock();
	  scene->renderScene();
	end = (double)clock();	
	dif = (end - start)/ CLOCKS_PER_SEC;
	printf ("Rendering time: %f seconds\n", dif);

	// write the image to an image file
	scene->getImage()->write("image.ppm");
}

void updateScene()
{
	// projection transform
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(30.0, width/height, 1, 1000);
	glMatrixMode(GL_MODELVIEW);

	Camera* camera = scene->getCamera();
	eye = camera->getEye();
	lookat = camera->getLookat();
	up = camera->getUp();

	Group* group = scene->getObjects();
	std::vector<Object*> objects = group->getObjects();
	for each (Object* obj in objects)
	{
		obj->preprocess();
	}

	sceneIsDirty = false;
}

	float pixels [512*512];
void drawObjects(Vector lookdir)
{
	Group* group = scene->getObjects();
	std::vector<Object*> objects = group->getObjects();
	
	//glClear(GL_STENCIL_BUFFER_BIT);
	//glClearStencil(0);
	//glEnable(GL_STENCIL_TEST);			
	glReadPixels(0, 0, 512, 512, GL_RED, GL_FLOAT, pixels);

	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	int index = 0;
	int numObjects = group->numObjects();
	
	for each (Object* obj in objects)
	{
		float red = (float)(index + 1.0f) / (numObjects + 1.0f);
		obj->rasterize(lookdir, red);
		index ++;
	}	

	glReadPixels(0, 0, 512, 512, GL_RED, GL_FLOAT, pixels);
	//glDisable(GL_STENCIL_TEST);
}

void display( )
{
	if (sceneIsDirty)
		updateScene();

	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	// camera transform
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(eye.x, eye.y, eye.z, lookat.x, lookat.y, lookat.z, up.x, up.y, up.z);
	
	Vector lookdir = lookat - eye;
	lookdir.normalize();
	drawObjects(lookdir);

	/*
    glBegin(GL_POINTS);

	for (int i=128; i<384; i++)
    {
        for (int j=128; j<384; j++)
        {
            //draw a white squared
            drawPoint(i,j, 1,1,1);
        }
    }
	glEnd();
	*/
glutSwapBuffers();
}

int main( int argc, char** argv )
{
	if (argc < 2) {
		cerr << "You need to give the program a scene" << endl;
		exit(1);
	}

	ifstream sceneFile( argv[1] );
	if ( !sceneFile.good() ) {
		cerr << "This scene file was bad: " << argv[ 1 ] << endl;
		exit(1);
	}

	double start,end, dif;

	// parse the scene file
	start = (double)clock();
	  std::string filename = argv[1];
	  Parser parser(sceneFile);
	  scene = parser.parseScene(filename);
	end = (double)clock();
	dif = (end - start)/ CLOCKS_PER_SEC;
	printf ("Parsing time: %f seconds\n", dif);

	//
	// create the glut window
	//
	glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE|GLUT_DEPTH);
	glutInitWindowSize(width, height);
	glutInitWindowPosition(100,100);
	main_window = glutCreateWindow("Ray Tracing");
    glPointSize(1);

	glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutReshapeFunc(reshape);
    glutIdleFunc(idle);

	// give control over to glut
	glutMainLoop();
	
	return 0;
}