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
#ifdef USING_OPENGL
  /*
  glPushMatrix();
    Vector u = lookdir.cross(norm) * 1000000.0;
    Vector v = norm.cross(u) * 1000000.0;

    glBegin(GL_QUADS);
    Vector vert = point.asVector() - u;
    glVertex3f(vert.x, vert.y, vert.z);
    vert = point.asVector() + v;
    glVertex3f(vert.x, vert.y, vert.z);
    vert = point.asVector() + u;
    glVertex3f(vert.x, vert.y, vert.z);
    vert = point.asVector() - v;
    glVertex3f(vert.x, vert.y, vert.z);
    glEnd();

  glPopMatrix();
  */

  // instead rasterize a large disk
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

#endif
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