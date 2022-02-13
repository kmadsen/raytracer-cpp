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
	double tmin,tmax,tymin,tymax,tzmin,tzmax;
	double divx,divy,divz;
	Point O = ray.pos;
	Vector V = ray.dir;

	divx = 1 / V.x;
	if(divx >= 0) {
		tmin = (corner0.x - O.x) * divx;
		tmax = (corner1.x - O.x) * divx;
	}
	else {
		tmin = (corner1.x - O.x) * divx;
		tmax = (corner0.x - O.x) * divx;
	}

	divy = 1 / V.y;
	if(divy >= 0) {
		tymin = (corner0.y - O.y) * divy;
		tymax = (corner1.y - O.y) * divy;
	}
	else {
		tymin = (corner1.y - O.y) * divy;
		tymax = (corner0.y - O.y) * divy;
	}

	if ( (tmin > tymax) || (tymin > tmax) )
		return;
	if (tymin > tmin)
		tmin = tymin;
	if (tymax < tmax)
		tmax = tymax;
	

	divz = 1 / V.z;
	if(divz >= 0) {
		tzmin = (corner0.z - O.z) * divz;
		tzmax = (corner1.z - O.z) * divz;
	}
	else {
		tzmin = (corner1.z - O.z) * divz;
		tzmax = (corner0.z - O.z) * divz;
	}

	if (tmin > tzmax || tzmin > tmax)
		return;
	if (tzmin > tmin)
		tmin = tzmin;
	if (tzmax < tmax)
		tmax = tzmax;

	hit.hit(tmin, this, material);
	hit.hit(tmax, this, material);
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