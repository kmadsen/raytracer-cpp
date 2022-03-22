#ifndef Ring_h
#define Ring_h

#include "Material.h"
#include "Object.h"
#include "Point.h"
#include "Primitive.h"
#include "Vector.h"

class Ring : public Primitive {
 public:
  Ring(std::shared_ptr<Material> _material, const Point& _center, const Vector& _norm,
       const double& _radiusInner, const double& _radiusOuter)
      : Primitive(_material),
        center(_center),
        norm(_norm),
        radiusInner(_radiusInner),
        radiusOuter(_radiusOuter) {}
  virtual ~Ring(void);

	ObjectType type() const { return ObjectType::TRing; }

  void preprocess(const RenderContext& context);
  void intersect(HitRecord& hit, const RenderContext& context,
                 const Ray& ray) const;
  void normal(Vector& normal, const RenderContext&, const Point& hitpos,
              const Ray& ray, const HitRecord& hit) const;
  void rasterize(const Vector& lookdir) const;

 private:
  Point center;
  Vector norm;
  double d;
  double radiusInner;
  double radiusInnerSquared;
  double radiusOuter;
  double radiusOuterSquared;
};

#endif  // Ring_h
