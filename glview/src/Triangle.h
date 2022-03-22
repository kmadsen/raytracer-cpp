#ifndef Triangle_h
#define Triangle_h

#include "Material.h"
#include "Object.h"
#include "Point.h"
#include "Primitive.h"
#include "Vector.h"

class Triangle : public Primitive {
 public:
  Triangle(std::shared_ptr<Material> _material, const Point& corner0, const Point& corner1,
           const Point& corner2)
      : Primitive(_material), p0(corner0), p1(corner1), p2(corner2) {}

  virtual ~Triangle(void);

	ObjectType type() const { return ObjectType::TTriangle; }

  void setBounds();
  void preprocess(const RenderContext& context);
  void intersect(HitRecord& hit, const RenderContext& context,
                 const Ray& ray) const;
  void normal(Vector& normal, const RenderContext&, const Point& hitpos,
              const Ray& ray, const HitRecord& hit) const;
  void rasterize(const Vector& lookdir) const;

  Point getP0() const { return p0; }
  Point getP1() const { return p1; }
  Point getP2() const { return p2; }

 private:
  Vector v0, v1;
  Point p0, p1, p2;
  Vector norm;
};

#endif  // Triangle_h
