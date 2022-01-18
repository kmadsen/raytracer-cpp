#ifndef Group_h
#define Group_h

#include "Object.h"
#include "RenderContext.h"
#include "HitRecord.h"
#include "Ray.h"
#include <vector>

class Group : public Object
{
public:
	Group(void);
	virtual ~Group(void);

	void addObject(Object* object);

	void preprocess();

	void intersect(HitRecord& hit, const RenderContext& context, const Ray& ray) const;

private:
	std::vector<Object*> objects;
};

#endif // Group_h
