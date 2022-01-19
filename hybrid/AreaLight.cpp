#include "AreaLight.h"
#include <cstdlib>
#include <ctime> 

AreaLight::~AreaLight(void)
{

}

void AreaLight::preprocess(const RenderContext& context)
{

}

double AreaLight::getLight(Color& light_color, Vector& light_direction,
							const RenderContext& context, const Point& pos) const
{
	light_color = color;

	Vector randEdge1 = edge1 * ((double)rand() / RAND_MAX);
	Vector randEdge2 = edge2 * ((double)rand() / RAND_MAX);	

	Point lightPoint = corner + randEdge1 + randEdge2;
	light_direction = lightPoint - pos;
	double length = light_direction.length();
	light_direction.normalize();
	return length;
}

void AreaLight::getLightInfo(Color& light_color, Vector& light_direction,
							 Point& light_position) const
{
	// TODO: NOT HANDLED
}
