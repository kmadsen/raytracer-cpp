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

#ifdef USING_OPENGL
	#include "GL/glut.h"
#endif 

class Sphere : public Primitive
{
public:
	Sphere(Material* _material, const Point& _center, const double& _radius)
		: center(_center), radius(_radius)
	{
		setMaterial(_material);
	}

	virtual ~Sphere(void);

	void preprocess();
	void intersect(HitRecord& hit, const RenderContext& context, const Ray& ray) const;
	void normal(Vector& normal, const RenderContext&, const Point& hitpos,
		const Ray& ray, const HitRecord& hit) const;
	void rasterize(const Vector& lookdir, const float& color);
private:
	Point center;
	double radius;
};

#endif // Shpere_h