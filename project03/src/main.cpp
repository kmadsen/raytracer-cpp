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

	// Create a scene from the file input
	std::string filename = argv[1];
	Parser parser(sceneFile);
	Scene* scene = parser.parseScene(filename);

	scene->preprocess();
	scene->renderScene();
	scene->getImage()->write("image.ppm");

	return 0;
}
