#ifndef RASTERSCENEMAPPER_H_
#define RASTERSCENEMAPPER_H_

#include <GL/glew.h>

#include "Point.h"
#include "Vector.h"
#include "Color.h"

#include "RasterScene.h"
#include "Camera.h"
#include "PinholeCamera.h"
#include "Light.h"
#include "PointLight.h"
#include "Group.h"
#include "BVHGroup.h"
#include "Triangle.h"
#include "Sphere.h"
#include "Plane.h"
#include "Disk.h"
#include "Ring.h"
#include "Box.h"
#include "Material.h"
#include "PhongMaterial.h"
#include "LambertianMaterial.h"
#include "DielectricMaterial.h"
#include "MetalMaterial.h"
#include "GlossymetalMaterial.h"

class RasterSceneMapper
{
 // Private because this is a static class
 private:
  RasterSceneMapper() { }
  ~RasterSceneMapper() { }
 public:
  static glm::vec3 fromVector(const Vector& vector);
  static glm::vec3 fromPoint(const Point& point);
  static glm::vec3 fromColor(const Color& color);

  static raster::Camera* fromPinholeCamera(const PinholeCamera* camera);
  static raster::Camera* fromCamera(const Camera* camera);

  static raster::PointLight* fromPointLight(const PointLight* light);

  static raster::Material* fromPhongMaterial(const PhongMaterial* phong);
	static raster::Material* fromLambertianMaterial(const LambertianMaterial* lambertian);
	static raster::Material* fromDielectricMaterial(const DielectricMaterial* dielectric);
	static raster::Material* fromMetalMaterial(const MetalMaterial* metal);
	static raster::Material* fromGlossymetalMaterial(const GlossymetalMaterial* glossymetal);
  static raster::Material* fromMaterial(const Material* material);

  static raster::Triangle* fromTriangle(const Triangle* triangle);
  static void fromSphere(const Sphere* sphere);
  static void fromPlane(const Plane* plane);
  static void fromDisk(const Disk* disk);
  static void fromRing(const Ring* ring);
  static void fromBox(const Box* box);
  static void fromObject(raster::SceneBuilder* sceneBuilder, const Object* object);
  static void fromGroup(raster::SceneBuilder* sceneBuilder, const Group* group);
  static void fromBVHGroup(raster::SceneBuilder* sceneBuilder, const BVHGroup* bvhgroup);

  static raster::Scene* fromScene(const Scene* scene);
};

#endif