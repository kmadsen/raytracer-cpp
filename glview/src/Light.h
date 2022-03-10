#ifndef Light_h
#define Light_h

class Color;
class Vector;
class Point;
class RenderContext;

enum LightType { TPoint, TArea, TDirection };

class Light
{
public:
	Light(void);
	virtual ~Light(void);

	virtual LightType type() const = 0;

	virtual void preprocess(const RenderContext& context) = 0;
	virtual double getLight(Color& light_color, Vector& light_direction,
		const RenderContext& context, const Point& pos) const = 0;
	virtual void getLightInfo(Color& light_color, Vector& light_direction,
		Point& position) const = 0;
};

#endif //Light_h