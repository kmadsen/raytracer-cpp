#ifndef RayTracerOpenGL_h
#define RayTracerOpenGL_h

#include "RayTracerImage.h"
#include "Scene.h"
#include "Vector.h"
#include "Parser.h"
#include "Image.h"

#include <iomanip>
#include <iostream>
#include <fstream>

using namespace std;

void RayTracerImage::start(int argc, char** argv) {
	// Create a scene from the file input
	ifstream sceneFile(argv[1]);
	std::string filename = argv[1];
	Parser parser(sceneFile);

	Scene* scene = parser.parseScene(filename);
	scene->preprocess();
	scene->renderScene();
	scene->getImage()->write("image.ppm");

	cout << "finished instant run: xdg-open image.ppm\n";
};

#endif // RayTracerOpenGL_h
