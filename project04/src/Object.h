#ifndef Object_h
#define Object_h

#include "HitRecord.h"
#include "RenderContext.h"
#include "Ray.h"
#include "Point.h"
#include "Color.h"
#include <math.h>
#include <cstdlib>

// #define USING_OPENGL
#ifdef USING_OPENGL
	#include "GL/glut.h"
#endif 

class Object
{
public:
	Object(void) 
	{ 
		_min = Point(HUGE_VAL);
		_max = Point(-HUGE_VAL);
		rastColor = Color(((float)rand() / RAND_MAX), ((float)rand() / RAND_MAX), ((float)rand() / RAND_MAX));
	}
	virtual ~Object(void);

	virtual void preprocess() = 0;
	virtual void intersect(HitRecord& hit, const RenderContext& context, const Ray& ray) const = 0;
	
	// override this to make opengl be able to rasterize the element
	virtual void rasterize(const Vector& lookdir, const float& red)
	{

	}

	Point getMin() const { return _min; }
	Point getMax() const { return _max; }
	void setMin(const Point& m) { _min = Point::min(m, _min); }
	void setMax(const Point& m) { _max = Point::max(m, _max); }

	Point getCenter()
	{
		return Point((_max.x + _min.x) / 2.0,
					 (_max.y + _min.y) / 2.0,
					 (_max.z + _min.z) / 2.0 );
	}

	Color rastColor;

private:
	Point _min;
	Point _max;
};

#endif // Object_h

