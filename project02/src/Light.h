#ifndef Light_h
#define Light_h

class Color;
class Vector;
class Point;
class RenderContext;

class Light
{
public:
	Light(void);
	virtual ~Light(void);

	virtual void preprocess() = 0;
	virtual double getLight(Color& light_color, Vector& light_direction,
		const RenderContext& context, const Point& pos) const = 0;
};

#endif //Light_h