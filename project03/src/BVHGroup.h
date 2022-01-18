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

	void preprocess();
	void intersect(HitRecord& hit, const RenderContext& context, const Ray& ray) const;
	double intersectBox(const Ray& ray) const; 
	//void addObjectToGroup(Object* object);
	void addObject(Object* object);

	Object* l_object;
	Object* r_object;
private:
};

#endif // Group_h