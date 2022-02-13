#include "Triangle.h"

Triangle::~Triangle(void)
{
}

void Triangle::preprocess(const RenderContext& context)
{
	Primitive::preprocess(context);
}

void Triangle::setBounds()
{
	setMin(Point::min(Point::min(p0, p1), p2));
	setMax(Point::max(Point::max(p0, p1), p2));	
}

void Triangle::rasterize(const Vector &lookdir) const
{
#ifdef USING_OPENGL
	glPushMatrix();
	  glBegin(GL_TRIANGLES);
	  
	    glNormal3f(norm.x, norm.y, norm.z);
	    glVertex3f(p0.x, p0.y, p0.z);
	    glVertex3f(p1.x, p1.y, p1.z);
	    glVertex3f(p2.x, p2.y, p2.z);
		
	    glNormal3f(-norm.x, -norm.y, -norm.z);
	    glVertex3f(p0.x, p0.y, p0.z);
	    glVertex3f(p1.x, p1.y, p1.z);
	    glVertex3f(p2.x, p2.y, p2.z);
	    glEnd();
	glPopMatrix();
#endif
}

void Triangle::intersect(HitRecord& hit, const RenderContext& context, const Ray& ray) const
{
	Vector u,v,w; // temporary edges and values
	double a,b,c,d; // temporary computing values

	u = ray.dir.cross(v1);
	a = v0.dot(u);

	if (abs(a) < SEPS)
		return; // no hit

	b = 1.0 / a;
	v = ray.pos - p2;
	c = b * (v.dot(u));

	if (c < 0 || c > 1)
		return; // no hit

	w = v.cross(v0);
	d = b * ray.dir.dot(w);
	if (d < 0 || c + d > 1)
		return; // no hit

	// finally find the distance where there is an intersection
	hit.hit(b * v1.dot(w), this, material);
}

void Triangle::normal(Vector& normal, const RenderContext&, const Point& hitpos,
                    const Ray& ray, const HitRecord& hit) const
{
	normal = norm;
}