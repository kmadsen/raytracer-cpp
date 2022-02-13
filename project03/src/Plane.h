#ifndef Plane_h
#define Plane_h


#include "Material.h"
#include "Vector.h"
#include "Point.h"
#include "Object.h"
#include "Primitive.h"

class Plane : public Primitive
{
public:
	Plane(Material* _material, const Vector& _norm, const Point& _point)
		: norm(_norm), point(_point) 
	{
		setMaterial(_material);
		norm.normalize(); //TODO put in preprocess
		d = norm.dot(point.asVector());
	}
	virtual ~Plane(void);

	void preprocess();
	void intersect(HitRecord& hit, const RenderContext& context, const Ray& ray) const;
	void normal(Vector& normal, const RenderContext&, const Point& hitpos,
		const Ray& ray, const HitRecord& hit) const;
private:
	Vector norm;
	Point point;
	double d;
};

#endif // Plane_h