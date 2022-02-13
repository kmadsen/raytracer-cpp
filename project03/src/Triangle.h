#ifndef Triangle_h
#define Triangle_h

#include "Material.h"
#include "Point.h"
#include "Object.h"
#include "Primitive.h"
#include "Vector.h"

class Triangle : public Primitive
{
public:
	Triangle(Material* material, const Point& corner0, 
		const Point& corner1, const Point& corner2)
		: p0(corner0), p1(corner1), p2(corner2)
	{
		setMaterial(material);
		v0 = p0 - p2;
		v1 = p1 - p2;
		norm = v0.cross(v1);
		norm.normalize();
	}

	virtual ~Triangle(void);

	void preprocess();
	void intersect(HitRecord& hit, const RenderContext& context, const Ray& ray) const;
	void normal(Vector& normal, const RenderContext&, const Point& hitpos,
		const Ray& ray, const HitRecord& hit) const;
private:
	Vector v0, v1;
	Point p0, p1, p2;
	Vector norm;
};

#endif // Triangle_h
