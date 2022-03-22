#ifndef Plane_h
#define Plane_h

#include "Material.h"
#include "Object.h"
#include "Point.h"
#include "Primitive.h"
#include "Vector.h"

class Plane : public Primitive {
 public:
  Plane(std::shared_ptr<Material> _material, const Vector& _norm, const Point& _point)
      : Primitive(_material), norm(_norm), point(_point) {}
  virtual ~Plane(void) {}

	ObjectType type() const { return ObjectType::TPlane; }

  void preprocess(const RenderContext& context);
  void intersect(HitRecord& hit, const RenderContext& context,
                 const Ray& ray) const;
  void normal(Vector& normal, const RenderContext&, const Point& hitpos,
              const Ray& ray, const HitRecord& hit) const;
  void rasterize(const Vector& lookdir) const;

 private:
  Vector norm;
  Point point;
  double d;
};

#endif  // Plane_h