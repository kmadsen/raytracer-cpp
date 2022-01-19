
#ifndef PointLight_h
#define PointLight_h

#include "Point.h"
#include "Color.h"
#include "Light.h"

class PointLight : public Light
{
public:
	PointLight(const Point& _position, const Color& _color)
		: position(_position), color(_color) {}
	virtual ~PointLight(void);

	void preprocess();
	double getLight(Color& light_color, Vector& light_direction,
		const RenderContext& context, const Point& pos) const;
private:
	Point position;
	Color color;
};

#endif // PointLight_h
