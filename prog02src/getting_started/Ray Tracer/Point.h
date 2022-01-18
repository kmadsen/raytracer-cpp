
#include "Vector.h"

#ifndef Point_h
#define Point_h

#include <math.h>

class Point
{
public:
	double x, y, z;
 
	Point() : x(0.0), y(0.0), z(0.0) 
	{}

	Point(const double& _x, const double& _y, const double& _z) 
		: x(_x), y(_y), z(_z) { } 

	virtual ~Point(void);

	// Operators
	inline Point operator + (const Vector &v) const
	{ return Point(x + v.x, y + v.y, z + v.z); }

	inline Vector operator - (const Point &p) const
    { return Vector(x - p.x, y - p.y, z - p.z); }

	// conversion
	inline Vector asVector() const
	{ return Vector(x, y, z); }
};

#endif // Point

