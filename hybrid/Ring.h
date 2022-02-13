#ifndef Ring_h
#define Ring_h

#include "Material.h"
#include "Point.h"
#include "Vector.h"
#include "Object.h"
#include "Primitive.h"

class Ring : public Primitive
{
public:
	Ring(Material* _material, const Point& _center, const Vector& _norm, 
		 const double& _radiusInner, const double& _radiusOuter)
		 : center(_center), norm(_norm), radiusInner(_radiusInner), radiusOuter(_radiusOuter)
	{
		setMaterial(_material);
		norm.normalize(); // TODO put in preprocess
	}
	virtual ~Ring(void);

	void preprocess(const RenderContext& context);
	void intersect(HitRecord& hit, const RenderContext& context, const Ray& ray) const;
	void normal(Vector& normal, const RenderContext&, const Point& hitpos,
                    const Ray& ray, const HitRecord& hit) const;
	void Ring::rasterize(const Vector &lookdir) const;
private:
	Point center;
	Vector norm;
	double radiusInner;
	double radiusOuter;
};

#endif // Ring_h
