#include "Disk.h"
#include "Constants.h"

Disk::~Disk(void)
{
}

void Disk::preprocess()
{}

void Disk::intersect(HitRecord& hit, const RenderContext& context, const Ray& ray) const
{
	// TODO make less Vector Point conversions.
	double product = norm.dot(ray.dir);
	if (abs(product) > EPSILON)
	{
		double d = norm.dot(center.asVector()); 
		double t = (d-norm.dot(ray.pos.asVector()))/product;
		
		Point p = ray.pos + ray.dir * t;
		Vector dist = p - center;
		if(dist.length() <= (radius))
		{
			hit.hit(t, this, material);
		}
	}
}

void Disk::normal(Vector& normal, const RenderContext&, const Point& hitpos,
				  const Ray& ray, const HitRecord& hit) const
{
	normal = norm;
}