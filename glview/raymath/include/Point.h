
#include "Vector.h"

#ifndef POINT_H_
#define POINT_H_

#include <math.h>
#include <algorithm>

class Point {
 public:
  double x, y, z;

  Point() : x(0.0), y(0.0), z(0.0) {}

  explicit Point(const double& value) : x(value), y(value), z(value) {}

  inline Point(const Point& copy) {
    x = copy.x;
    y = copy.y;
    z = copy.z;
  }

  Point(const double& _x, const double& _y, const double& _z)
      : x(_x), y(_y), z(_z) {}

  virtual ~Point(void) {}

  inline Point operator+(const Vector& v) const {
    return Point(x + v.x, y + v.y, z + v.z);
  }

  inline Point operator+(const Point& v) const {
    return Point(x + v.x, y + v.y, z + v.z);
  }

  inline Vector operator-(const Point& p) const {
    return Vector(x - p.x, y - p.y, z - p.z);
  }

  inline Point& operator+=(const Vector& a) {
    x += a.x;
    y += a.y;
    z += a.z;
    return *this;
  }

  inline Point& operator-=(const Vector& a) {
    x -= a.x;
    y -= a.y;
    z -= a.z;
    return *this;
  }

  inline Point operator/(const double& v) const {
    return Point(x / v, y / v, z / v);
  }

  inline Vector asVector() const { return Vector(x, y, z); }

  inline static Point min(const Point& p0, const Point& p1) {
    double x = std::min(p0.x, p1.x);
    double y = std::min(p0.y, p1.y);
    double z = std::min(p0.z, p1.z);
    return Point(x, y, z);
  }

  inline static Point max(const Point& p0, const Point& p1) {
    double x = std::max(p0.x, p1.x);
    double y = std::max(p0.y, p1.y);
    double z = std::max(p0.z, p1.z);
    return Point(x, y, z);
  }
};

#endif  // POINT_H_
