#include "Box.h"
#include "Constants.h"
#include <math.h>

Box::~Box(void)
{
}

void Box::preprocess(const RenderContext& context)
{
	Primitive::preprocess(context);

	setMin(Point::min(corner0, corner1));
	setMax(Point::max(corner0, corner1));
}

void Box::rasterize(const Vector &lookdir) const
{
#ifdef USING_OPENGL
	glPushMatrix();
	  Vector scale = corner1 - corner0;
	  glTranslatef(corner0.x + scale.x / 2.0, corner0.y + scale.y / 2.0, corner0.z + scale.z / 2.0);
	  glScalef(abs(scale.x), abs(scale.y), abs(scale.z));
	  glutSolidCube(1.0);
	glPopMatrix();
#endif
}

void Box::intersect(HitRecord& hit, const RenderContext& context, const Ray& ray) const
{
	double tmin, tmax, tymin, tymax, tzmin, tzmax;
	tmin  =  (bounds[ray.sign[0]].x   - ray.pos.x) * ray.inv_direction.x;
	tmax  =  (bounds[1-ray.sign[0]].x - ray.pos.x) * ray.inv_direction.x;
	tymin =  (bounds[ray.sign[1]].y   - ray.pos.y) * ray.inv_direction.y;
	tymax =  (bounds[1-ray.sign[1]].y - ray.pos.y) * ray.inv_direction.y;
	if ( (tmin > tymax) || (tymin > tmax) )
		return;
	if (tymin > tmin)
		tmin = tymin;
	if (tymax < tmax)
		tmax = tymax;
	tzmin =  (bounds[ray.sign[2]].z   - ray.pos.z) * ray.inv_direction.z;
	tzmax =  (bounds[1-ray.sign[2]].z - ray.pos.z) * ray.inv_direction.z;
	if ( (tmin > tzmax) || (tzmin > tmax) )
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