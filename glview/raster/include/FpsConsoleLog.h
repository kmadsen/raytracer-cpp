#ifndef FPSCONSOLELOG_H_
#define FPSCONSOLELOG_H_

#include <chrono>

class FpsConsoleLog {
 public:
  FpsConsoleLog();
  virtual ~FpsConsoleLog();

  void frameEnd();

 private:
  int frames;
  std::chrono::high_resolution_clock::time_point lastTime;
};

#endif  // FPSCONSOLELOG_H_
