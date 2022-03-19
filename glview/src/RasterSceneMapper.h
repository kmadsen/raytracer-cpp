#ifndef RASTERSCENEMAPPER_H_
#define RASTERSCENEMAPPER_H_

#include "Point.h"
#include "Vector.h"
#include "Color.h"

#include "RasterScene.h"
#include "Camera.h"
#include "PinholeCamera.h"
#include "Light.h"
#include "PointLight.h"

#include <GL/glew.h>

static glm::vec3 fromVector(const Vector& vector) {
  return glm::vec3(vector.x, vector.y, vector.z);
}

static glm::vec3 fromPoint(const Point& point) {
  return glm::vec3(point.x, point.y, point.z);
}

static glm::vec3 fromColor(const Color& color) {
  return glm::vec3(color.r(), color.g(), color.b());
}

static raster::Camera* fromPinholeCamera(const PinholeCamera* camera) {
  return new raster::Camera(
    fromPoint(camera->getEye()),
    fromPoint(camera->getLookat()),
    fromVector(camera->getUp()),
    camera->getFieldOfView()
  );
}

static raster::Camera* fromCamera(const Camera* camera) {
  switch (camera->type())
  {
  case CameraType::TPinhole:
    return fromPinholeCamera(static_cast<const PinholeCamera*>(camera));
    break;
  default:
    fprintf(stderr, "Unrecognized camera type %s\n", camera->name());
    exit(EXIT_FAILURE);
  }
}

static raster::PointLight* fromPointLight(const PointLight* light) {
  Color color;
  Vector direction;
	Point position;
  light->getLightInfo(color, direction, position);
  return new raster::PointLight(
    fromPoint(position),
    fromColor(color)
  );
}

static raster::Scene* fromScene(const Scene* scene) {
  auto width = scene->getImage()->getXresolution();
  auto height = scene->getImage()->getYresolution();

  auto fromLights = scene->getLights();
  std::vector<raster::PointLight*> toPointLights;
  for (Light* light : scene->getLights()) {
    if (light->type() == LightType::TPoint) {
      toPointLights.push_back(fromPointLight(static_cast<const PointLight*>(light)));
    } else {
      fprintf(stderr, "Unrecognized light type %s\n", light->name());
      exit(EXIT_FAILURE);
    }
  }
  auto rasterSceneBuilder = raster::SceneBuilder()
    .width(width)
    .height(height)
    .camera(fromCamera(scene->getCamera()))
    .pointLights(toPointLights);
  return rasterSceneBuilder.build();
}

#endif