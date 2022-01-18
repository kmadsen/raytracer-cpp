#include "Box.h"
#include "Constants.h"
#include <math.h>

Box::~Box(void)
{
}

void Box::preprocess()
{}

void Box::intersect(HitRecord& hit, const RenderContext& context, const Ray& ray) const
{
	double dirX = ray.dir.x;
	double dirY = ray.dir.y;
	double dirZ = ray.dir.z;

	double tmin, tmax, tymin, tymax, tzmin, tzmax;
	double divx = 1 / dirX;
	if (divx >= 0) 
	{
		tmin = (corner0.x - ray.pos.x) * divx;
		tmax = (corner1.x - ray.pos.x) * divx;
	}
	else 
	{
		tmin = (corner1.x - ray.pos.x) * divx;
		tmax = (corner0.x - ray.pos.x) * divx;
	}
	double divy = 1 / dirY;
	if (divy >= 0)
	{
		tymin = (corner0.y - ray.pos.y) * divy;
		tymax = (corner1.y - ray.pos.y) * divy;
	}
	else
	{
		tymin = (corner1.y - ray.pos.y) * divy;
		tymax = (corner0.y - ray.pos.y) * divy;
	}

	if ( (tmin > tymax) || (tymin > tmax) )
		return;
	if (tymin > tmin)
		tmin = tymin;
	if (tymax < tmax)
		tmax = tymax;

	double divz = 1 / dirZ;
	if (divz >= 0)
	{
		tzmin = (corner0.z - ray.pos.z) * divz;
		tzmax = (corner1.z - ray.pos.z) * divz;
	}
	else
	{
		tzmin = (corner1.z - ray.pos.z) * divz;
		tzmax = (corner0.z - ray.pos.z) * divz;
	}

	if ( (tmin > tzmax) || (tzmin > tmax) )
		return;
	if (tzmin > tmin)
		tmin = tzmin;
	if (tzmax < tmax)
		tmax = tzmax;
	hit.hit(tmin, this, material);
}


void Box::normal(Vector& normal, const RenderContext&, const Point& hitpos,
				 const Ray& ray, const HitRecord& hit) const
{
	// this assumes axis aligned.
	if ( abs(hitpos.x - corner0.x) < EPSILON )
		normal = Vector(-1.0, 0.0, 0.0);
	else if ( abs(hitpos.x - corner1.x) < EPSILON )
		normal = Vector(1.0, 0.0, 0.0 );
	else if ( abs( hitpos.y - corner0.y) < EPSILON )
		normal = Vector(0.0, -1.0, 0.0);
	else if ( abs( hitpos.y - corner1.y) < EPSILON )
		normal = Vector(0.0, 1.0, 0.0 );
	else if ( abs( hitpos.z - corner0.z) < EPSILON )
		normal = Vector(0.0, 0.0, -1.0);
	else 
		normal = Vector(0.0, 0.0, 1.0);

}