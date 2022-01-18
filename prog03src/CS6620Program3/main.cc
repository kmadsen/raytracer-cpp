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

#include <iomanip>
#include <iostream>
#include <fstream>
#include <time.h>

using namespace std;

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
	  Scene* scene = parser.parseScene(filename);
	end = (double)clock();
	dif = (end - start)/ CLOCKS_PER_SEC;
	printf ("Parsing time: %f seconds\n", dif);

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

	cout<<"Press any key to exit: \n";
    cin.get();

	return 0;
}