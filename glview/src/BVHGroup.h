#ifndef BVHGroup_h
#define BVHGroup_h

#include "Group.h"
#include "RenderContext.h"
#include "HitRecord.h"
#include "Ray.h"
#include "Point.h"
#include <vector>

class BVHGroup : public Group
{
public:
	BVHGroup();
	BVHGroup(int _maxChildren);
	virtual ~BVHGroup(void);

	ObjectType type() const { return ObjectType::TBVHGroup; }

	void preprocess(const RenderContext& context);
	void intersect(HitRecord& hit, const RenderContext& context, const Ray& ray) const;
	double intersectBox(const Ray& ray) const;
	void rasterize(const Vector &lookdir) const;

	void addObject(Object* object);
	Object* getLeftObjects() const { return l_object; }
	Object* getRightObjects() const { return r_object; }

	Object* l_object;
	Object* r_object;
private:
};

#endif // Group_h