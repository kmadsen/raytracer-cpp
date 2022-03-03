#include "Disk.h"

#include "Intersect.h"

Disk::Disk(Material* _material, const Point& _center, const Vector& _norm,
           const double& _radius)
    : Primitive(_material), center(_center), norm(_norm), radius(_radius) {}

void Disk::preprocess(const RenderContext& context) {
  Primitive::preprocess(context);
  norm.normalize();
  d = norm.dot(center.asVector());
  radiusSquared = radius * radius;
}

void Disk::intersect(HitRecord& hit, const RenderContext& context,
                     const Ray& ray) const {
  double t;
  if (Intersect::disk(ray, norm, center, d, radiusSquared, t)) {
    hit.hit(t, this, material);
  }
}

void Disk::normal(Vector& normal, const RenderContext&, const Point& hitpos,
                  const Ray& ray, const HitRecord& hit) const {
  normal = norm;
}

void Disk::rasterize(const Vector& lookdir) const {
#ifdef USING_OPENGL
  glPushMatrix();
  Vector zAxis = Vector(0.0, 0.0, 1.0);
  Vector rotAxis = zAxis.cross(norm);
  rotAxis.normalize();
  double rotAngle = acos(norm.dot(zAxis));
  rotAngle = rotAngle * 180.0 / PI;

  glTranslatef(center.x, center.y, center.z);
  glRotatef(rotAngle, rotAxis.x, rotAxis.y, rotAxis.z);

  gluDisk(quadratic, 0.0, radius, 50, 10);
  glPopMatrix();
#endif
}