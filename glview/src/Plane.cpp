#include "Plane.h"
#include "Constants.h"
#include <math.h>

Plane::~Plane(void)
{
}

void Plane::preprocess(const RenderContext& context)
{
	Primitive::preprocess(context);
}

void Plane::rasterize(const Vector &lookdir) const
{
#ifdef USING_OPENGL
	/*
	glPushMatrix();
	  Vector u = lookdir.cross(norm) * 1000000.0;
	  Vector v = norm.cross(u) * 1000000.0;

	  glBegin(GL_QUADS);
	  Vector vert = point.asVector() - u;
	  glVertex3f(vert.x, vert.y, vert.z);
	  vert = point.asVector() + v;
	  glVertex3f(vert.x, vert.y, vert.z);
	  vert = point.asVector() + u;
	  glVertex3f(vert.x, vert.y, vert.z);
	  vert = point.asVector() - v;
	  glVertex3f(vert.x, vert.y, vert.z);
	  glEnd();
    
	glPopMatrix();
	*/

	// instead rasterize a large disk
	glPushMatrix();
	  Vector zAxis = Vector(0.0, 0.0, 1.0);
	  Vector rotAxis = zAxis.cross(norm);
	  rotAxis.normalize();
	  double rotAngle = acos(norm.dot(zAxis));
	  rotAngle = rotAngle * 180 / PI;
	
	  glTranslatef(point.x, point.y, point.z);
	  glRotatef(rotAngle, rotAxis.x, rotAxis.y, rotAxis.z);

	  gluDisk(quadratic, 0.0, 100.0, 50, 10);
	glPopMatrix();
	
#endif
}

void Plane::intersect(HitRecord& hit, const RenderContext& context, const Ray& ray) const
{
	double denom = norm.dot(ray.dir);
	
	if ( abs( denom ) > EPSILON ) {
		double t = (d - norm.dot(ray.pos.asVector()) )  / denom;
		hit.hit( t, this, material );
	}
}


void Plane::normal(Vector& normal, const RenderContext&, const Point& hitpos,
				   const Ray& ray, const HitRecord& hit) const
{
	normal = norm;
}