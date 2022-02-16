#include "Ring.h"
#include "Constants.h"

Ring::~Ring(void) {}

void Ring::preprocess(const RenderContext& context) {
  Primitive::preprocess(context);
}

double dot(const Vector& lhs, const Point& rhs) {
    return double(lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z);
}

void Ring::intersect(HitRecord& hit, const RenderContext& context,
                     const Ray& ray) const {
  // TODO make less Vector Point conversions.

  double product = norm.dot(ray.dir);
  if (abs(product) > EPSILON) {
    double d = dot(norm, center);
    double t = (d - dot(norm, ray.pos)) / product;

    Point p = ray.pos + ray.dir * t;
    Vector dist = p - center;
    if (dist.length() <= (radiusOuter) && dist.length() >= (radiusInner)) {
      hit.hit(t, this, material);
    }
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