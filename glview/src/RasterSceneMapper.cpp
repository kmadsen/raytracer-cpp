
#include "RasterSceneMapper.h"

#include <iostream>

glm::vec3 RasterSceneMapper::fromVector(const Vector& vector) {
  return glm::vec3(vector.x, vector.y, vector.z);
}

glm::vec3 RasterSceneMapper::fromPoint(const Point& point) {
  return glm::vec3(point.x, point.y, point.z);
}

glm::vec3 RasterSceneMapper::fromColor(const Color& color) {
  return glm::vec3(color.r(), color.g(), color.b());
}

raster::Camera* RasterSceneMapper::fromPinholeCamera(const PinholeCamera* camera) {
  return new raster::Camera(
    fromPoint(camera->getEye()),
    fromPoint(camera->getLookat()),
    fromVector(camera->getUp()),
    camera->getFieldOfView()
  );
}

raster::Camera* RasterSceneMapper::fromCamera(const Camera* camera) {
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

raster::PointLight* RasterSceneMapper::fromPointLight(const PointLight* light) {
  Color color;
  Vector direction;
	Point position;
  light->getLightInfo(color, direction, position);
  return new raster::PointLight(
    fromPoint(position),
    fromColor(color)
  );
}

raster::Material* RasterSceneMapper::fromPhongMaterial(const PhongMaterial* phong) {
  return new raster::Material(
    fromColor(phong->getColor()),
    phong->getKa(),
    phong->getKd()
  );
}

raster::Material* RasterSceneMapper::fromLambertianMaterial(const LambertianMaterial* lambertian) {
  return new raster::Material(
    fromColor(lambertian->getColor()),
    lambertian->getKa(),
    lambertian->getKd()
  );
}

raster::Material* RasterSceneMapper::fromDielectricMaterial(const DielectricMaterial* dielectric) {
  std::cout << "Handle fromDielectricMaterial" << std::endl;
  return new raster::Material();
}

raster::Material* RasterSceneMapper::fromMetalMaterial(const MetalMaterial* metal) {
  std::cout << "Handle fromMetalMaterial" << std::endl;
  return new raster::Material();
}

raster::Material* RasterSceneMapper::fromGlossymetalMaterial(const GlossymetalMaterial* glossymetal) {
  std::cout << "Handle fromGlossyMetalMaterial" << std::endl;
  return new raster::Material();
}

raster::Material* RasterSceneMapper::fromMaterial(const Material* material) {
  switch (material->type())
  {
  case MaterialType::TPhong:
    return fromPhongMaterial(static_cast<const PhongMaterial*>(material));
    break;
  case MaterialType::TLambertian:
    return fromLambertianMaterial(static_cast<const LambertianMaterial*>(material));
    break;
  case MaterialType::TDielectric:
    return fromDielectricMaterial(static_cast<const DielectricMaterial*>(material));
    break;
  case MaterialType::TMetal:
    return fromMetalMaterial(static_cast<const MetalMaterial*>(material));
    break;
  case MaterialType::TGlossyMetal:
    return fromGlossymetalMaterial(static_cast<const GlossymetalMaterial*>(material));
    break;
  default:
    fprintf(stderr, "Unrecognized material type %s\n", material->name());
    exit(EXIT_FAILURE);
  }
}

raster::Triangle* RasterSceneMapper::fromTriangle(const Triangle* triangle) {
  return new raster::Triangle(
    fromMaterial(triangle->getMaterial().get()),
    fromPoint(triangle->getP0()),
    fromPoint(triangle->getP1()),
    fromPoint(triangle->getP2())
  );
}

void RasterSceneMapper::fromSphere(const Sphere* sphere) {
  std::cout << "Handle fromSphere" << std::endl;
}

void RasterSceneMapper::fromPlane(const Plane* plane) {
  std::cout << "Handle fromPlane" << std::endl;
}

void RasterSceneMapper::fromDisk(const Disk* disk) {
  std::cout << "Handle fromDisk" << std::endl;
}

void RasterSceneMapper::fromRing(const Ring* ring) {
  std::cout << "Handle fromRing" << std::endl;
}

void RasterSceneMapper::fromBox(const Box* box) {
  std::cout << "Handle fromBox" << std::endl;
}

void RasterSceneMapper::fromObject(raster::SceneBuilder* sceneBuilder, const Object* object) {
  switch (object->type())
  {
  case ObjectType::TGroup:
    fromGroup(sceneBuilder, static_cast<const Group*>(object));
    break;
  case ObjectType::TBVHGroup:
    fromBVHGroup(sceneBuilder, static_cast<const BVHGroup*>(object));
    break;
  case ObjectType::TTriangle:
    sceneBuilder->addTriangle(fromTriangle(static_cast<const Triangle*>(object)));
    break;
  case ObjectType::TSphere:
    fromSphere(static_cast<const Sphere*>(object));
    break;
  case ObjectType::TPlane:
    fromPlane(static_cast<const Plane*>(object));
    break;
  case ObjectType::TDisk:
    fromDisk(static_cast<const Disk*>(object));
    break;
  case ObjectType::TRing:
    fromRing(static_cast<const Ring*>(object));
    break;
  case ObjectType::TBox:
    fromBox(static_cast<const Box*>(object));
    break;
  default:
    fprintf(stderr, "Unrecognized object type %s\n", object->name());
    exit(EXIT_FAILURE);
  }
}

void RasterSceneMapper::fromGroup(raster::SceneBuilder* sceneBuilder, const Group* group) {
  std::cout << "Handle fromGroup" << std::endl;
  for (auto object : group->getObjects()) {
    fromObject(sceneBuilder, object);
  }
}

void RasterSceneMapper::fromBVHGroup(raster::SceneBuilder* sceneBuilder, const BVHGroup* bvhgroup) {
  std::cout << "Handle fromBVHGroup" << std::endl;
  for (auto object : bvhgroup->getObjects()) {
    fromObject(sceneBuilder, object);
  }
}

raster::Scene* RasterSceneMapper::fromScene(const Scene* scene) {
  auto width = scene->getImage()->getXresolution();
  auto height = scene->getImage()->getYresolution();

  auto fromLights = scene->getLights();
  std::vector<raster::PointLight*> toPointLights;
  for (auto light : scene->getLights()) {
    if (light->type() == LightType::TPoint) {
      toPointLights.push_back(fromPointLight(static_cast<const PointLight*>(light)));
    } else {
      fprintf(stderr, "Unrecognized light type %s\n", light->name());
      exit(EXIT_FAILURE);
    }
  }

  auto rasterSceneBuilder = new raster::SceneBuilder();
  (*rasterSceneBuilder)
    .width(width)
    .height(height)
    .camera(fromCamera(scene->getCamera()))
    .pointLights(toPointLights);

  fromGroup(rasterSceneBuilder, scene->getGroup());

  return rasterSceneBuilder->build();
}
