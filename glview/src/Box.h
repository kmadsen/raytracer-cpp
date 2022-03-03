#ifndef Box_h
#define Box_h

#include "Material.h"
#include "Object.h"
#include "Point.h"
#include "Primitive.h"

class Box : public Primitive {
 public:
  Box(Material* _material, const Point& _corner0, const Point& _corner1)
      : Primitive(_material), corner0(_corner0), corner1(_corner1) {}

  virtual ~Box(void);

  void preprocess(const RenderContext& context);
  void intersect(HitRecord& hit, const RenderContext& context,
                 const Ray& ray) const;
  void normal(Vector& normal, const RenderContext&, const Point& hitpos,
              const Ray& ray, const HitRecord& hit) const;
  void rasterize(const Vector& lookdir) const;

 private:
  Point corner0;
  Point corner1;
};

#endif  // Box_h
