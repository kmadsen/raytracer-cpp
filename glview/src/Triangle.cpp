#include "Triangle.h"
#include "Intersect.h"

Triangle::~Triangle(void) {

}

void Triangle::preprocess(const RenderContext& context) {
  Primitive::preprocess(context);
  v0 = p1 - p0;
  v1 = p2 - p0;
  norm = v0.cross(v1);
  norm.normalize();
}

void Triangle::setBounds() {
  setMin(Point::min(Point::min(p0, p1), p2));
  setMax(Point::max(Point::max(p0, p1), p2));
}

void Triangle::rasterize(const Vector& lookdir) const {
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

void Triangle::intersect(HitRecord& hit, const RenderContext& context,
                         const Ray& ray) const {
	double t;
	if (Intersect::triangle(ray, p0, v0, v1, t)) {
	  hit.hit(t, this, material);
	}
}

void Triangle::normal(Vector& normal, const RenderContext&, const Point& hitpos,
                      const Ray& ray, const HitRecord& hit) const {
  normal = norm;
}