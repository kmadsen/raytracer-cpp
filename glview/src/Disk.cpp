#include "Disk.h"
#include "Constants.h"

Disk::~Disk(void)
{
}

void Disk::preprocess(const RenderContext& context)
{
	Primitive::preprocess(context);
}

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

void Disk::rasterize(const Vector &lookdir) const
{
#ifdef USING_OPENGL
	glPushMatrix();
	Vector zAxis = Vector(0.0, 0.0, 1.0);
	Vector rotAxis = zAxis.cross(norm);
	rotAxis.normalize();
	double rotAngle = acos(norm.dot(zAxis));
	rotAngle = rotAngle * 180.0 / PI;
	
	glTranslatef(center.x, center.y, center.z);
	glRotatef(rotAngle, rotAxis.x, rotAxis.y, rotAxis.z);

	gluDisk(quadratic, 0.0, radius, 50, 10);
	glPopMatrix();
#endif
}