#include <fstream>
#include <iostream>

#include "RayTracerGlfw.h"
#include "RayTracerGlut.h"
#include "RayTracerImage.h"

using namespace std;

/**
 * @brief The entry point for the raytracer.
 *
 * @param argc Up to 2 arguments allowed.
 * @param argv
 * 	 arg[1] = prog01.scn
 * 	 arg[2] = instant | <empty launches OpenGL window>
 * @return int 0 .
 */
int main(int argc, char** argv) {
  if (argc < 2) {
    cerr << "You need to give the program a scene" << endl;
    exit(1);
  }

  ifstream sceneFile(argv[1]);
  if (!sceneFile.good()) {
    cerr << "This scene file was bad: " << argv[1] << endl;
    exit(1);
  }

  if (argc > 2) {
    if (argv[2] == std::string("instant")) {
			auto rayTracerImage = RayTracerImage();
			rayTracerImage.start(argc, argv);
      return 0;
    } else if (argv[2] == std::string("glut")) {
      auto rayTracerGlut = RayTracerGlut();
      rayTracerGlut.start(argc, argv);
      return 0;
    } else {
      cout << "unrecognized command: " << argv[2] << "\n";
      cout << "  try \"instant\" or do not pass a second argument\n";
    }
  }

  auto rayTracerGlew = RayTracerGlfw();
  rayTracerGlew.start(argc, argv);

  return 0;
}
