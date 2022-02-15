#ifndef Vector_h
#define Vector_h

#include <math.h>

class Vector {
 public:
  double x, y, z;

  Vector() : x(0.0), y(0.0), z(0.0) {}

  Vector(const double &_x, const double &_y, const double &_z)
      : x(_x), y(_y), z(_z) {}

  virtual ~Vector() {}

  inline bool operator==(const Vector &a) const {
    return x == a.x && y == a.y && z == a.z;
  }

  inline bool operator!=(const Vector &a) const {
    return x != a.x || y != a.y || z != a.z;
  }

  inline Vector operator-() const { return Vector(-x, -y, -z); }

  inline Vector operator+(const Vector &a) const {
    return Vector(x + a.x, y + a.y, z + a.z);
  }

  inline Vector operator+(const double &a) const {
    return Vector(x + a, y + a, z + a);
  }

  inline Vector operator-(const Vector &a) const {
    return Vector(x - a.x, y - a.y, z - a.z);
  }

  inline Vector operator-(const double &a) const {
    return Vector(x - a, y - a, z - a);
  }

  inline Vector operator*(const Vector &a) const {
    return Vector(x * a.x, y * a.y, z * a.z);
  }

  inline Vector operator*(const double &a) const {
    return Vector(x * a, y * a, z * a);
  }

  inline Vector &operator*=(const Vector &a) {
    x *= a.x;
    y *= a.y;
    z *= a.z;
    return *this;
  }

  inline Vector &operator*=(const double &a) {
    x *= a;
    y *= a;
    z *= a;
    return *this;
  }

  inline Vector &operator+=(const Vector &a) {
    x += a.x;
    y += a.y;
    z += a.z;
    return *this;
  }

  inline Vector &operator+=(const double &a) {
    x += a;
    y += a;
    z += a;
    return *this;
  }

  inline Vector &operator-=(const Vector &a) {
    x -= a.x;
    y -= a.y;
    z -= a.z;
    return *this;
  }

  inline Vector &operator-=(const double &a) {
    x -= a;
    y -= a;
    z -= a;
    return *this;
  }

  inline Vector operator/(const Vector &a) const {
    return Vector(x / a.x, y / a.y, z / a.z);
  }

  inline Vector operator/(const double &a) const {
    return Vector(x / a, y / a, z / a);
  }

  inline Vector &operator/=(const Vector &a) {
    x /= a.x;
    y /= a.y;
    z /= a.z;
    return *this;
  }

  inline Vector &operator/=(const double &a) {
    x /= a;
    y /= a;
    z /= a;
    return *this;
  }

  inline double dot(const Vector &a) const {
    return double(x * a.x + y * a.y + z * a.z);
  }

  inline Vector cross(const Vector &v1) const {
    Vector c;
    c.x = (y * v1.z) - (z * v1.y);
    c.y = (z * v1.x) - (x * v1.z);
    c.z = (x * v1.y) - (y * v1.x);
    return c;
  }

  inline double length() const { return sqrt(x * x + y * y + z * z); }

  inline double normalize() {
    double length = sqrt(x * x + y * y + z * z);
    x = x / length;
    y = y / length;
    z = z / length;
    return length;
  }
};

#endif  // Vector_h
