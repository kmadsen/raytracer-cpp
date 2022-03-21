#include "Plane.h"

#include <math.h>

#include "Constants.h"
#include "Intersect.h"

void Plane::preprocess(const RenderContext& context) {
  Primitive::preprocess(context);
  norm.normalize();
  d = norm.dot(point.asVector());
}

void Plane::rasterize(const Vector& lookdir) const {
  GLUquadricObj* quadratic = gluNewQuadric();
  gluQuadricNormals(quadratic, GLU_SMOOTH);
  gluQuadricTexture(quadratic, GL_TRUE);

  glPushMatrix();
  Vector zAxis = Vector(0.0, 0.0, 1.0);
  Vector rotAxis = zAxis.cross(norm);
  rotAxis.normalize();
  double rotAngle = acos(norm.dot(zAxis));
  rotAngle = rotAngle * 180 / PI;

  glTranslatef(point.x, point.y, point.z);
  glRotatef(rotAngle, rotAxis.x, rotAxis.y, rotAxis.z);

  gluDisk(quadratic, 0.0, 100.0, 50, 10);
  glPopMatrix();

  gluDeleteQuadric(quadratic);
}

void Plane::intersect(HitRecord& hit, const RenderContext& context,
                      const Ray& ray) const {
  double t;
  if (Intersect::plane(ray, norm, point, d, t)) {
    hit.hit(t, this, material);
  }
}

void Plane::normal(Vector& normal, const RenderContext&, const Point& hitpos,
                   const Ray& ray, const HitRecord& hit) const {
  normal = norm;
}