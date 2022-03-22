#ifndef Scene_h
#define Scene_h

#include <vector>

#include "Color.h"

class Camera;
class Background;
class Light;
class Object;
class Image;
class HitRecord;
class RenderContext;
class Color;
class Ray;
class Group;
class Vector;
class Point;

class Scene {
 public:
  Scene(void);
  virtual ~Scene(void);

  void preprocess();

  void addLight(Light* _light) { lights.push_back(_light); }

  std::vector<Light*> getLights() const { return lights; }

  void setAmbient(const Color& _ambient) { ambient = _ambient; }

  Color getAmbient() const { return ambient; }

  void setBackground(Background* _background) { background = _background; }

  Background* getBackground() { return background; }

  void setCamera(Camera* _camera) { camera = _camera; }

  Camera* getCamera() const { return camera; }

  void setImage(Image* _image) { image = _image; }

  // I made this function so the pinhole camera could map pixels to rays.
  // I'm not sure if this is the best way to do this.
  Image* getImage() const { return image; }

  void setImageResolution(int xres, int yres);

  void setMaxRayDepth(const int& _maxRayDepth) { maxRayDepth = _maxRayDepth; }

  int getMaxRayDepth() const { return maxRayDepth; }

  void setSamples(const int& _samples) { samples = _samples; }

  void setMinAttenuation(const double& _minAtten) { minAtten = _minAtten; }

  double getMinAttenuation() const { return minAtten; }

  void setObjects(Group* _group) { group = _group; }

  Group* getGroup() const { return group; }

  void traceRay(Color& result, const RenderContext& context, const Ray& ray,
                HitRecord& hitRec, const Color& atten, int depth) const;

  bool traceShadowRay(const RenderContext& context, const Ray& ray,
                      HitRecord& hitRec) const;
  void renderScene();
  void renderRasterizedScene(const RenderContext& context);
  void setUpLightsOpenGL();

  void renderOpenGL(const Vector& lookdir);

  int* getStencils() { return stencils; }
  float* getPixels() { return pixels; }

 private:
  Color ambient;
  Background* background;
  Camera* camera;
  Image* image = nullptr;
  int maxRayDepth;
  int samples = 0;
  double minAtten;
  Group* group;
  std::vector<Light*> lights;
  int* stencils = nullptr;
  float* pixels = nullptr;
};

#endif  // #ifndef Scene_h
