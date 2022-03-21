#include "Group.h"

Group::Group(void) {}

Group::~Group(void) {
  for (auto obj : objects) {
    delete obj;
  }
  objects.clear();
}

void Group::addObject(Object* object) { pushBack(object); }

void Group::preprocess(const RenderContext& context) {
  for (auto obj : objects) {
    obj->preprocess(context);
    setMin(obj->getMin());
    setMax(obj->getMax());
  }
}

void Group::rasterize(const Vector& lookdir) const {
#ifdef USING_OPENGL
  int i = 0;
  for (auto obj : objects) {
    glStencilFunc(GL_ALWAYS, i, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    obj->setOpenGLMaterial();
    obj->rasterize(lookdir);
    i++;
  }
#endif
}

void Group::intersect(HitRecord& hit, const RenderContext& context,
                      const Ray& ray) const {
  for (auto obj : objects) {
    obj->intersect(hit, context, ray);
  }
}
