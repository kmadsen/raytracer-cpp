#include "Sphere.h"

Sphere::~Sphere(void)
{
}

void Sphere::rasterize(const Vector& lookdir, const float& color)
{
#ifdef USING_OPENGL
	glColor3f(color, 0.0, 0.0);
	glPushMatrix();
	  glTranslatef(center.x, center.y, center.z);
	  glutSolidSphere(radius, 30, 30);
	glPopMatrix();
#endif
}

void Sphere::preprocess()
{
	setMin(Point(center.x - radius, center.y - radius, center.z - radius));
	setMax(Point(center.x + radius, center.y + radius, center.z + radius));
}

void Sphere::intersect(HitRecord& hit, const RenderContext& context, const Ray& ray) const
{
	Vector disc;
	Vector e = ray.pos.asVector();
	Vector d = ray.dir;
	Vector c = center.asVector();

	Vector ec = c-e; // vector from eye to center of sphere.

	// sphere center is behind the eye so return false for now
	if(ec.dot(d) < 0){
		return;
	} else {
		Vector pec = d*(d.dot(ec)); // projection of ec vector onto ray
		double dist = (pec-ec).length();

		// projection point is not inside the sphere.
		// if dist == r then it is on the surface.
		if( dist >= radius ){
			return;
		} else {
			// calculate the two distances from the eye to a sphere intersections.
			double temp = sqrt(radius*radius - dist*dist);
			double t0 = pec.length() - temp;
			double t1 = pec.length() + temp;
			hit.hit(t0, this, material); 
			hit.hit(t1, this, material);
		}
	}
}

void Sphere::normal(Vector& normal, const RenderContext&, const Point& hitpos,
					const Ray& ray, const HitRecord& hit) const
{
	normal = (hitpos.asVector() - center.asVector()) * radius;
	normal.normalize();
}