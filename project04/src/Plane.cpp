#include "Plane.h"
#include "Constants.h"
#include <math.h>

Plane::~Plane(void)
{
}

void Plane::preprocess()
{}

void Plane::rasterize(const Vector& lookdir, const float& color)
{
#ifdef USING_OPENGL
	glColor3f(color, 0.0, 0.0);
	glPushMatrix();
	  Vector u = lookdir.cross(norm) * 100000.0;
	  Vector v = norm.cross(u) * 100000.0;

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