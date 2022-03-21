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

	void preprocess(const RenderContext& context);

	void intersect(HitRecord& hit, const RenderContext& context, const Ray& ray) const;
	void rasterize(const Vector &lookdir) const;
	Object* popBack()
	{
		if (objects.size() > 0) {
			Object* obj = objects.back();
			objects.pop_back();
			return obj;
		}
		else
			return 0x0;
	}
	void pushBack(Object* obj)
	{
		objects.push_back(obj);
	}
	int numObjects()
	{
		return (int)objects.size();
	}

	std::vector<Object*> getObjects() { return objects; }
protected:
	std::vector<Object*> objects;
};

#endif // Group_h
