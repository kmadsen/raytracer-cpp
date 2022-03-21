#ifndef Object_h
#define Object_h

#include "HitRecord.h"
#include "RenderContext.h"
#include "Ray.h"
#include "Point.h"
#include "Color.h"
#include <math.h>
#include <cstdlib>

#ifdef USING_OPENGL
	#include "GL/glut.h"
#endif 

class Object
{
public:
	Object(void) 
	{ 
		bounds[0] = Point(HUGE_VAL);
		bounds[1] = Point(-HUGE_VAL);
		rastColor = Color(((float)rand() / RAND_MAX), ((float)rand() / RAND_MAX), ((float)rand() / RAND_MAX));

#ifdef USING_OPENGL
		quadratic=gluNewQuadric();			
		gluQuadricNormals(quadratic, GLU_SMOOTH);	
		gluQuadricTexture(quadratic, GL_TRUE);	
#endif
	}
	virtual ~Object(void);

	virtual void setBounds() {}
	virtual void preprocess(const RenderContext& context) = 0;
	virtual void intersect(HitRecord& hit, const RenderContext& context, const Ray& ray) const = 0;
	
	// override this to make opengl be able to rasterize the element
	virtual void setOpenGLMaterial() const {}
	virtual void rasterize(const Vector &lookdir) const {}


	Point getMin() const { return bounds[0]; }
	Point getMax() const { return bounds[1]; }
	void setMin(const Point& m) { bounds[0] = Point::min(m, bounds[0]); }
	void setMax(const Point& m) { bounds[1] = Point::max(m, bounds[1]); }

	Point getCenter()
	{
		return Point((bounds[1].x + bounds[0].x) / 2.0,
					 (bounds[1].y + bounds[0].y) / 2.0,
					 (bounds[1].z + bounds[0].z) / 2.0 );
	}

	Color rastColor;
protected:
	Point bounds[2];
#ifdef USING_OPENGL
GLUquadricObj *quadratic;
#endif

};

#endif // Object_h

