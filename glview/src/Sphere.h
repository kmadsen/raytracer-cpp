#ifndef Sphere_h
#define Sphere_h

#include "Material.h"
#include "Point.h"
#include "Object.h"
#include "Primitive.h"
#include "RenderContext.h"
#include "Ray.h"
#include "Vector.h"
#include "HitRecord.h"

class Sphere : public Primitive
{
public:
	Sphere(std::shared_ptr<Material> _material, const Point& _center, const double& _radius)
		: Primitive(_material), center(_center), radius(_radius)
	{}

	virtual ~Sphere(void);

	ObjectType type() const { return ObjectType::TSphere; }

	void preprocess(const RenderContext& context);
	void intersect(HitRecord& hit, const RenderContext& context, const Ray& ray) const;
	void normal(Vector& normal, const RenderContext&, const Point& hitpos,
		const Ray& ray, const HitRecord& hit) const;
	void rasterize(const Vector &lookdir) const;
private:
	Point center;
	double radius;
};

#endif // Shpere_h