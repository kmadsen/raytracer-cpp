#ifndef Object_h
#define Object_h

#include "HitRecord.h"
#include "RenderContext.h"
#include "Ray.h"

class Object
{
public:
	Object(void);
	virtual ~Object(void);

	virtual void preprocess() = 0;
	virtual void intersect(HitRecord& hit, const RenderContext& context, const Ray& ray) const = 0;
};

#endif // Object_h

