#ifndef HitRecord_h
#define HitRecord_h

#include <memory>

#include "Constants.h"

#include "Material.h"

class Primitive;

class HitRecord
{
public:
	HitRecord(double _min)
		: min(_min) 
	{
		// set to NULL
		primitive = 0x0;
	}

	virtual ~HitRecord(void);

	bool hit(double t, const Primitive* hit_prim, const std::shared_ptr<Material> hit_matl) 
	{ 
		if(t > EPSILON && t < min){
			min = t;
			primitive = hit_prim;
			material = hit_matl;
			return true;
		} else {
			return false;
		}
	}
	double getMinT() const {
		return min;
	}
	const Primitive* getPrimitive() const {
		return primitive;
	}
	const std::shared_ptr<Material> getMaterial() const {
		return material;
	}
private:
	const Primitive* primitive;
	std::shared_ptr<Material> material;
	double min;
};

#endif // HitRecord_h
