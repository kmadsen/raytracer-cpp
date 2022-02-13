#include "Triangle.h"

Triangle::~Triangle(void)
{
}

void Triangle::preprocess()
{}

void Triangle::intersect(HitRecord& hit, const RenderContext& context, const Ray& ray) const
{
	Vector e1,e2,u,v,w; // temporary edges and values
	double a,b,c,d; // temporary computing values

	e1 = p1 - p0; // edge from p0 to p1
	e2 = p2 - p0; // edge from p0 to p2
	u = ray.dir.cross(e2);
	a = e1.dot(u);

	if ((int)a == 0)
		return; // no hit

	b = 1/a;
	v = ray.pos - p0;
	c = b * (v.dot(u));

	if (c < 0 || c > 1)
		return; // no hit

	w = v.cross(e1);
	d = b * ray.dir.dot(w);
	if (d < 0 || c + d > 1)
		return; // no hit

	// finally find the distance where there is an intersection
	hit.hit(b * e2.dot(w), this, material);
}

void Triangle::normal(Vector& normal, const RenderContext&, const Point& hitpos,
                    const Ray& ray, const HitRecord& hit) const
{
	normal = norm;
}