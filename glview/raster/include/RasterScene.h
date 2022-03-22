#ifndef RASTERSCENE_H_
#define RASTERSCENE_H_

#include <stdio.h>
#include <vector>
#include <algorithm>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace raster {

class Material {
 public:
  glm::vec3 color;
  float kAmbient;
  float kDiffuse;

  Material() {
    color = glm::vec3(1.0f);
    kAmbient = 0.3f;
    kDiffuse = 0.5f;
  }

  Material(glm::vec3 _color, float _kAmbient, float _kDiffuse)
      : color(_color), kAmbient(_kAmbient), kDiffuse(_kDiffuse) {}
};

class Triangle {
 public:
  glm::vec3 p0;
  glm::vec3 p1;
  glm::vec3 p2;
  Material* material;

  Triangle(Material* _material, glm::vec3 _p0, glm::vec3 _p1, glm::vec3 _p2)
      : material(_material), p0(_p0), p1(_p1), p2(_p2) {}
};

class Camera {
 public:
  glm::vec3 eye;
  glm::vec3 center;
  glm::vec3 up;
  float fieldOfView;
  glm::mat4 view;

  Camera(glm::vec3 _eye, glm::vec3 _center, glm::vec3 _up, float _fov)
      : eye(_eye), center(_center), up(_up), fieldOfView(_fov) {
    this->view = glm::lookAt(eye, center, up);
  }
};

class PointLight {
 public:
  glm::vec3 position;
  glm::vec3 color;

  PointLight(glm::vec3 _position, glm::vec3 _color)
      : position(_position), color(_color) {}
};

class Scene {
 private:
  Scene& operator=(const Scene&) { return *this; }

 public:
  int width, height;
  Camera* camera;
  std::vector<PointLight*> pointLights;
  std::vector<Triangle*> triangles;

  virtual ~Scene() {
    delete camera;
    std::for_each(pointLights.cbegin(), pointLights.cend(),
                  [](PointLight* pointLight) { delete pointLight; });
  }

  friend class SceneBuilder;

  const float aspectRatio() const { return width / static_cast<float>(height); }
};

class SceneBuilder {
 private:
  Scene* scene;

 public:
  SceneBuilder() { scene = new Scene(); }
  ~SceneBuilder() {}

  Scene* build() { return scene; }

  SceneBuilder& width(const int& _width) {
    scene->width = _width;
    return *this;
  }

  SceneBuilder& height(const int& _height) {
    scene->height = _height;
    return *this;
  }

  SceneBuilder& camera(Camera* _camera) {
    scene->camera = _camera;
    return *this;
  }

  SceneBuilder& pointLights(std::vector<PointLight*> _pointLights) {
    scene->pointLights = _pointLights;
    return *this;
  }

  SceneBuilder& addTriangle(Triangle* triangle) {
    scene->triangles.push_back(triangle);
    return *this;
  }
};

}  // namespace raster

#endif  // RASTERSCENE_H_
