#include "Plane.h"
#include "Constants.h"
#include <math.h>

Plane::~Plane(void)
{
}

void Plane::preprocess()
{}

void Plane::intersect(HitRecord& hit, const RenderContext& context, const Ray& ray) const
{
	double product = norm.dot(ray.dir);
	if ( abs(product) > EPSILON )
	{
		double d, t;
		d = -(norm.dot(point.asVector()));
		t = -(d + norm.dot(ray.pos.asVector())/product); 
		hit.hit(t, this, material);
	}
}


void Plane::normal(Vector& normal, const RenderContext&, const Point& hitpos,
				   const Ray& ray, const HitRecord& hit) const
{
	normal = norm;
}