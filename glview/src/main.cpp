#include <fstream>
#include <iostream>
#include <signal.h>
#include <execinfo.h>

#include "RayTracerGlfw.h"
#include "RayTracerGlut.h"
#include "RayTracerImage.h"

using std::fprintf;
using std::cout;
using std::cerr;
using std::endl;
using std::ifstream;

// Found that you do not need this on Linux.
//
// https://www.freedesktop.org/software/systemd/man/coredumpctl.html
//
// This will start gdb on the last core dump and you can have an
// interactive debugger for the core.
//  $ coredumpctl debug
//  $ (gdb) backtrace
//
// Example 2. Invoke gdb on the last core dump
// void handler(int sig) {
//   size_t size;
//   void* array[10];
//   size = backtrace(array, 10);
//   backtrace_symbols_fd(array, size, STDERR_FILENO);
//   exit(EXIT_FAILURE);
// }

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
  // signal(SIGSEGV, handler);
  // raise(SIGSEGV); test the handler

  if (argc < 2) {
    cerr << "You need to give the program a scene" << endl;
    exit(EXIT_FAILURE);
  }

  ifstream sceneFile(argv[1]);
  if (!sceneFile.good()) {
    cerr << "This scene file was bad: " << argv[1] << endl;
    exit(EXIT_FAILURE);
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
      cout << "unrecognized command: " << argv[2] << "\n"
        << "  try \"instant\", \"glut\" or do not pass a second argument\n";
    }
  }

  auto rayTracerGlew = RayTracerGlfw();
  rayTracerGlew.start(argc, argv);

  return 0;
}
