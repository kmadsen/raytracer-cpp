#ifndef Object_h
#define Object_h

#include "HitRecord.h"
#include "RenderContext.h"
#include "Ray.h"
#include "Point.h"
#include <math.h>

class Object
{
public:
	Object(void) 
	{ 
		_min = Point(HUGE_VAL);
		_max = Point(-HUGE_VAL);
	}
	virtual ~Object(void);

	virtual void preprocess() = 0;
	virtual void intersect(HitRecord& hit, const RenderContext& context, const Ray& ray) const = 0;

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
private:
	Point _min;
	Point _max;
};

#endif // Object_h

