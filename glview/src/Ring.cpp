#include "Ring.h"

#include "Intersect.h"

Ring::~Ring(void) {}

void Ring::preprocess(const RenderContext& context) {
  Primitive::preprocess(context);
  norm.normalize();
  d = norm.dot(center.asVector());
  radiusInnerSquared = radiusInner * radiusInner;
  radiusOuterSquared = radiusOuter * radiusOuter;
}

double dot(const Vector& lhs, const Point& rhs) {
  return double(lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z);
}

void Ring::intersect(HitRecord& hit, const RenderContext& context,
                     const Ray& ray) const {
  double t;
  if (Intersect::ring(ray, norm, center, d, radiusInnerSquared,
                      radiusOuterSquared, t)) {
    hit.hit(t, this, material);
  }
}

void Ring::normal(Vector& normal, const RenderContext&, const Point& hitpos,
                  const Ray& ray, const HitRecord& hit) const {
  normal = norm;
}

void Ring::rasterize(const Vector& lookdir) const {
#ifdef USING_OPENGL
  glPushMatrix();
  Vector zAxis = Vector(0.0, 0.0, 1.0);
  Vector rotAxis = zAxis.cross(norm);
  rotAxis.normalize();
  double rotAngle = acos(norm.dot(zAxis));
  rotAngle = rotAngle * 180 / PI;

  glTranslatef(center.x, center.y, center.z);
  glRotatef(rotAngle, rotAxis.x, rotAxis.y, rotAxis.z);

  gluDisk(quadratic, radiusInner, radiusOuter, 800, 60);
  glPopMatrix();
#endif
}