#ifndef Primitive_h
#define Primitive_h

#include <memory>

#include "Object.h"

class Material;

class Primitive : public Object {
 public:
  Primitive(std::shared_ptr<Material> _material) : material(_material) {}
  ~Primitive(void) {
    material = nullptr;
  }

  virtual void preprocess(const RenderContext& context);
  virtual void intersect(HitRecord& hit, const RenderContext& context,
                         const Ray& ray) const;
  virtual void normal(Vector& normal, const RenderContext&, const Point& hitpos,
                      const Ray& ray, const HitRecord& hit) const = 0;

  void setOpenGLMaterial() const;

  std::shared_ptr<Material> getMaterial() { return material; }

 protected:
  std::shared_ptr<Material> material;
  Primitive(const Primitive&);
  Primitive& operator=(const Primitive&);
};

#endif  // Primitive_h
