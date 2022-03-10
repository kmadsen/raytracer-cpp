#ifndef RASTERWINDOW_H_
#define RASTERWINDOW_H_

class FrameBufferTexture;
namespace raster {
class Scene;
}

class RasterWindow {
 public:
  RasterWindow() {}
  virtual ~RasterWindow(void);

  void start(raster::Scene* scene);
  void resize(int width, int height);

 private:
  FrameBufferTexture* frameBufferTexture;
};

#endif  // RASTERWINDOW_H_
