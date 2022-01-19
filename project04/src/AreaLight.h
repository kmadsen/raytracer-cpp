#ifndef AreaLight_h
#define AreaLight_h

#include "Light.h"
#include "Vector.h"
#include "Point.h"
#include "Color.h"

class RenderContext;

class AreaLight : public Light
{
public:
	AreaLight(const Point& _corner, const Vector& _edge1, const Vector& _edge2, const Color& _color ) 
		: corner(_corner), edge1(_edge1), edge2(_edge2), color(_color)
	{

	}
	virtual ~AreaLight(void);

	void preprocess();
	double getLight(Color& light_color, Vector& light_direction,
		const RenderContext& context, const Point& pos) const;
private:
	Point corner;
	Vector edge1;
	Vector edge2;
	Color color;
};

#endif // AreaLight_h
