#include "Primitive.h"

Primitive::Primitive(void)
{
}

Primitive::~Primitive(void)
{
}

void Primitive::preprocess(const RenderContext& context)
{
	material->preprocess(context);
}

void Primitive::intersect(HitRecord& hit, const RenderContext& context, const Ray& ray) const
{
}

void Primitive::setOpenGLMaterial() const
{
	material->setOpenGLMaterial();
}
