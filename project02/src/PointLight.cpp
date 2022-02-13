#include "PointLight.h"
#include "Color.h"
#include "Vector.h"
#include "RenderContext.h"
#include "Point.h"

PointLight::~PointLight(void)
{
}

void PointLight::preprocess()
{

}

double PointLight::getLight(Color& light_color, Vector& light_direction,
							const RenderContext& context, const Point& pos) const
{
	light_color = color;
	Vector dir = position-pos;
	double len = dir.normalize();
	light_direction = dir;
	return len;
}
