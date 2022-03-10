#include "FpsConsoleLog.h"

#include <iostream>

using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;

using std::cout;
using std::endl;
using std::flush;
using std::string;

FpsConsoleLog::FpsConsoleLog() {
  lastTime = high_resolution_clock::now();
  cout << "Frames per second measuring..." << flush;
}

FpsConsoleLog::~FpsConsoleLog() {
  // After finishing, finally add a new line
  cout << endl << flush;
}

void FpsConsoleLog::frameEnd() {
  frames++;
  auto current = high_resolution_clock::now();
  auto seconds = duration_cast<duration<double>>(current - lastTime).count();
  if (seconds >= 1.0) {
    double fps = frames / seconds;
    cout << "\r" << flush;
    cout << "Frames: " << frames << " Seconds: " << seconds
      << " FPS: " << fps << flush;
    lastTime = current;
    frames = 0;
  }
}
