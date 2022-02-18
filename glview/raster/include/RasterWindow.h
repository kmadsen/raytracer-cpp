#ifndef RASTERWINDOW_H_
#define RASTERWINDOW_H_

class RasterWindow {
 public:
  RasterWindow() {}
  virtual ~RasterWindow(void) {}

  void start(int argc, char** argv);
};

#endif  // RASTERWINDOW_H_
