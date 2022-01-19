#include "Group.h"

Group::Group(void)
{
}

Group::~Group(void)
{
}

void Group::addObject(Object* object)
{
	pushBack(object);
}

void Group::preprocess(const RenderContext& context)
{
	int numObjects = objects.size();
	objectsAsArray = new Object*[numObjects];
	int i = 0;
	for each (Object* o in objects) {
		o->preprocess(context);
		setMin(o->getMin());
		setMax(o->getMax());

		objectsAsArray[i++] = o;
	}
}

void Group::rasterize(const Vector &lookdir) const
{
#ifdef USING_OPENGL
	int numObjects = objects.size();
	for (int i = 0; i < numObjects; i++) {
	    glStencilFunc(GL_ALWAYS, i, 0xFF);                  
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

		objectsAsArray[i]->setOpenGLMaterial();
		objectsAsArray[i]->rasterize(lookdir);
	}	
#endif
}

void Group::intersect(HitRecord& hit, const RenderContext& context, const Ray& ray) const
{
	for each (Object* o in objects)
		o->intersect(hit, context, ray);
}
