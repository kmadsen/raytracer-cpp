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
		: pos(_pos), dir(_dir)
	{
		inv_direction = Vector(1.0/_dir.x, 1.0/_dir.y, 1.0/_dir.z);
		sign[0] = (inv_direction.x < 0);
		sign[1] = (inv_direction.y < 0);
		sign[2] = (inv_direction.z < 0);
	}

	virtual ~Ray(void)
	{
	}

	Point pos;
	Vector dir;
	Vector inv_direction;
	int sign[3];
};

#endif // Ray_h

