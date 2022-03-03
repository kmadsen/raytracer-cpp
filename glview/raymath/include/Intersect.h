#ifndef INTERSECT_H_
#define INTERSECT_H_

#include "Ray.h"
#include "Constants.h"

#include <math.h>

class Intersect {
 public:
  Intersect() {}
  virtual ~Intersect() {}

  static bool plane(
    const Ray& ray,
    const Vector& normal,
    const Point& center,
    const double &d,
    double &t
  ) {
    bool hit = false;
    double denom = normal.dot(ray.dir);
    if (abs(denom) > EPSILON) {
      Point p = ray.pos;
      double e = normal.x * p.x + normal.y * p.y + normal.z * p.z;
      t = (d - e) / denom;
      hit = true;
    }
    return hit;
  }

  static bool disk(
    const Ray& ray,
    const Vector& normal,
    const Point& center,
    const double& d,
    const double& radiusSquared,
    double &t
  ) {
    bool hit = false;
    if (plane(ray, normal, center, d, t)) {
      Point p = ray.pos + ray.dir * t;
      Vector dist = p - center;
      double d2 = dist.dot(dist);
      hit = d2 <= radiusSquared;
    }
    return hit;
  }

  static bool ring(
    const Ray& ray,
    const Vector& normal,
    const Point& center,
    const double& d,
    const double& radiusInnerSquared,
    const double& radiusOuterSquared,
    double &t
  ) {
    bool hit = false;
    if (plane(ray, normal, center, d, t)) {
      Point p = ray.pos + ray.dir * t;
      Vector dist = p - center;
      double d2 = dist.dot(dist);
      hit = d2 <= radiusOuterSquared && d2 >= radiusInnerSquared;
    }
    return hit;
  }
};

#endif  // INTERSECT_H_
