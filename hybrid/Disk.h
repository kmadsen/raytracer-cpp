#ifndef Disk_h
#define Disk_h

#include "Material.h"
#include "Point.h"
#include "Vector.h"
#include "Object.h"
#include "Primitive.h"


class Disk : public Primitive
{
public:
	Disk(Material* _material, const Point& _center, const Vector& _norm, const double& _radius)
		: center(_center), norm(_norm), radius(_radius)
	{
		setMaterial(_material);
		norm.normalize(); // TODO put in preprocess
	}
	virtual ~Disk(void);

	void preprocess(const RenderContext& context);
	void intersect(HitRecord& hit, const RenderContext& context, const Ray& ray) const;
	void normal(Vector& normal, const RenderContext&, const Point& hitpos,
		const Ray& ray, const HitRecord& hit) const;
	void rasterize(const Vector &lookdir) const;
private:
	Point center;
	Vector norm;
	double radius;
};

#endif // Disk_h
