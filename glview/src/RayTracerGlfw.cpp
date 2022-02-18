#include "RayTracerGlfw.h"
#include "RasterWindow.h"
#include "Parser.h"
#include "Scene.h"
#include "Image.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <time.h>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <thread>
#include <vector>

#define USING_OPENGL

using namespace std;

void RayTracerGlfw::start(int argc, char** argv) {
  std::ifstream sceneFile(argv[1]);

  // parse the scene file
  std::string filename = argv[1];
  Parser parser(sceneFile);
  auto scene = parser.parseScene(filename);

  auto width = scene->getImage()->getXresolution();
  auto height = scene->getImage()->getYresolution();
  
  auto rasterWindow = RasterWindow();
  rasterWindow.start(argc, argv);
}
