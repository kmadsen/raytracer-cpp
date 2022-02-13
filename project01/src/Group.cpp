#include "Group.h"

Group::Group(void)
{
}

Group::~Group(void)
{
}

void Group::addObject(Object* object)
{
	objects.push_back(object);
}

void Group::preprocess()
{

}

void Group::intersect(HitRecord& hit, const RenderContext& context, const Ray& ray) const
{
	for (auto o = objects.begin(); o != objects.end(); ++o)
	{
		(*o)->intersect(hit, context, ray);
	}
}
