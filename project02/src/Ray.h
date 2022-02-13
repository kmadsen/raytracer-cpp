#ifndef Ray_h
#define Ray_h

#include "Point.h"
#include "Vector.h"

class Ray
{
public:
	Ray() {
		pos = Point();
		dir = Vector();
	}

	Ray(const Point& _pos, const Vector& _dir)
		: pos(_pos), dir(_dir) {}

	virtual ~Ray(void);

	Point pos;
	Vector dir;
};

#endif // Ray_h

