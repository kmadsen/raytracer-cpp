#ifndef RASTERSCENE_H_
#define RASTERSCENE_H_

#include <stdio.h>

#include <algorithm>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace raster {

class Camera {
 public:
  glm::vec3 eye;
  glm::vec3 center;
  glm::vec3 up;
  float fieldOfView;
  glm::mat4 view;

  Camera(glm::vec3 _eye, glm::vec3 _center, glm::vec3 _up, float _fov) {
    this->eye = _eye;
    this->center = _center;
    this->up = _up;
    this->fieldOfView = _fov;
    this->view = glm::lookAt(eye, center, up);
  }
};

class PointLight {
 public:
  glm::vec3 position;
  glm::vec3 color;

  PointLight(glm::vec3 _position, glm::vec3 _color) {
    this->position = _position;
    this->color = _color;
  }
};

class Scene {
 private:
  int _width, _height;
  Camera* _camera;
  std::vector<PointLight*> _pointLights;

  Scene& operator=(const Scene&) { return *this; }

 public:
  virtual ~Scene() {
    delete _camera;
    std::for_each(
      _pointLights.cbegin(),
      _pointLights.cend(),
      [](PointLight* pointLight) { delete pointLight; });
  }

  friend class SceneBuilder;

  const int& width() const { return _width; }
  const int& height() const { return _height; }
  const Camera* camera() const { return _camera; }
  const std::vector<PointLight*> pointLights() const { return _pointLights; }

  const float aspectRatio() const {
    return _width / static_cast<float>(_height);
  }
};

class SceneBuilder {
 private:
  Scene* scene;

 public:
  SceneBuilder() {
    scene = new Scene();
  }
  ~SceneBuilder() {}

  Scene* build() { return scene; }

  SceneBuilder& width(const int& _width) {
    scene->_width = _width;
    return *this;
  }

  SceneBuilder& height(const int& _height) {
    scene->_height = _height;
    return *this;
  }

  SceneBuilder& camera(Camera* _camera) {
    scene->_camera = _camera;
    return *this;
  }

  SceneBuilder& pointLights(std::vector<PointLight*> _pointLights) {
    scene->_pointLights = _pointLights;
    return *this;
  }
};

}  // namespace raster

#endif  // RASTERSCENE_H_
