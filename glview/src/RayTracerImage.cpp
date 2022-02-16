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
#include <regex>

using namespace std;

void RayTracerImage::start(int argc, char** argv) {
	// Create a scene from the file input
	ifstream sceneFile(argv[1]);
	string filename = argv[1];
	Parser parser(sceneFile);

	Scene* scene = parser.parseScene(filename);
	scene->preprocess();
	scene->renderScene();

	// Put the result image near the input scene file.
	auto rgx = regex("(.*\\/)(.*.scn)");
	string scenePath(argv[1]);
	smatch matches;
	if (regex_search(scenePath, matches, rgx)) {
		filename = string(matches[1]) + filename;
	}

	scene->getImage()->write(filename);

	cout << "finished instant run: xdg-open " << filename << "\n";
};

#endif // RayTracerOpenGL_h
