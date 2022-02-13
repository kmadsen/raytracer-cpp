
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

	Point(const double& value) : x(value), y(value), z(value) 
	{}

	inline Point(const Point& copy) 
	{
		x = copy.x;
		y = copy.y;
		z = copy.z;
	}

	Point(const double& _x, const double& _y, const double& _z) 
		: x(_x), y(_y), z(_z) { } 

	virtual ~Point(void);

	// Operators
	inline Point operator + (const Vector &v) const
	{ return Point(x + v.x, y + v.y, z + v.z); }

	inline Point operator + (const Point &v) const
	{ return Point(x + v.x, y + v.y, z + v.z); }

	inline Vector operator - (const Point &p) const
    { return Vector(x - p.x, y - p.y, z - p.z); }

	inline Point& operator+=(const Vector& a) {
		x += a.x; y += a.y; z += a.z;
		return *this;
	}

	inline Point& operator-=(const Vector& a) {
		x -= a.x; y -= a.y; z -= a.z;
		return *this;
	}

	inline Point operator / (const double &v) const
	{ return Point(x / v, y / v, z / v); }

	// conversion
	inline Vector asVector() const
	{ return Vector(x, y, z); }

	inline static Point min(const Point& p0, const Point& p1)
	{
		double x = p0.x < p1.x ? p0.x : p1.x;
		double y = p0.y < p1.y ? p0.y : p1.y;
		double z = p0.z < p1.z ? p0.z : p1.z;
		return Point(x, y, z);
	}

	inline static Point max(const Point& p0, const Point& p1)
	{
		double x = p0.x > p1.x ? p0.x : p1.x;
		double y = p0.y > p1.y ? p0.y : p1.y;
		double z = p0.z > p1.z ? p0.z : p1.z;
		return Point(x, y, z);
	}
};

#endif // Point

