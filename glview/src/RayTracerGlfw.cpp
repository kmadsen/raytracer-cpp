#include "RayTracerGlfw.h"
#include "RasterWindow.h"
#include "Parser.h"
#include "Scene.h"
#include "Image.h"
#include "RasterSceneMapper.h"

#include <fstream>

using std::string;
using std::ifstream;

void RayTracerGlfw::start(int argc, char** argv) {
  ifstream sceneFile(argv[1]);

  // parse the scene file
  string filename = argv[1];
  Parser* parser = new Parser(sceneFile);
  auto scene = parser->parseScene(filename);
  auto rasterScene = RasterSceneMapper::fromScene(scene);
  auto rasterWindow = RasterWindow();
  rasterWindow.start(rasterScene);
  delete rasterScene;
  delete parser;
  delete scene;
}
