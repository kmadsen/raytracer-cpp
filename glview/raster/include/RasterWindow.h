#ifndef RASTERWINDOW_H_
#define RASTERWINDOW_H_

class FrameBufferTexture;

class RasterWindow {
 public:
  RasterWindow() {}
  virtual ~RasterWindow(void);

  void start(int argc, char** argv);
  void resize(int width, int height);
 private:
  FrameBufferTexture* frameBufferTexture;
};

#endif  // RASTERWINDOW_H_
