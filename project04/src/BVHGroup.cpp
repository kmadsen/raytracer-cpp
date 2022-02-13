#include "BVHGroup.h"
#include "Box.h"
#include "Ray.h"

#include "PhongMaterial.h"
#include "Color.h"

BVHGroup::BVHGroup(void)
{ 
	l_object = 0x0;
	r_object = 0x0;
}

BVHGroup::~BVHGroup(void)
{

}

void BVHGroup::addObject(Object* object)
{
	object->preprocess();
	setMin(object->getMin());
	setMax(object->getMax());

	pushBack(object);
}

double splitValue(Object* obj, int axis) {
	switch(axis) {
		case 0:
			return obj->getCenter().x;
		case 1:
			return obj->getCenter().y;
		case 2:
			return obj->getCenter().z;
	}
	return 0.0; // you're an idiot..
}

// this is a binary split funtion using stl.
void splitGroup(BVHGroup* group, int axis) {
	if (group->numObjects() <= 4) {
		return;
	}

	double groupSplit = splitValue(group, axis);

	// split about the axis
	BVHGroup* lhs = new BVHGroup();
	BVHGroup* rhs = new BVHGroup();

	int numObjects = group->numObjects();
	for (int i = 0;i < numObjects; i ++) {
		Object* obj = group->popBack();

		double objectSplit = splitValue(obj, axis);

		if (objectSplit < groupSplit) {
			lhs->addObject(obj);
		} 
		else {
			rhs->addObject(obj);
		}
	}

	group->pushBack(lhs);
	group->pushBack(rhs);

	int nextAxis = (axis + 1) % 3;
	splitGroup(lhs, nextAxis);
	splitGroup(rhs, nextAxis);
}

// this is a binary split funtion using Pointers.
void splitPointerGroup(BVHGroup* group, int axis) {
	if (group->numObjects() <= 2) {
		group->l_object = group->popBack();
		group->r_object = group->popBack();
		return;
	}

	double groupSplit = splitValue(group, axis);

	// split about the axis
	group->l_object = new BVHGroup();
	group->r_object = new BVHGroup();

	int numObjects = group->numObjects();
	for (int i = 0;i < numObjects; i ++) {
		Object* obj = group->popBack();

		double objectSplit = splitValue(obj, axis);

		if (objectSplit < groupSplit) {
			((BVHGroup*)group->l_object)->addObject(obj);
		} 
		else {
			((BVHGroup*)group->r_object)->addObject(obj);
		}
	}

	int nextAxis = (axis + 1) % 3;
	splitPointerGroup((BVHGroup*)group->l_object, nextAxis);
	splitPointerGroup((BVHGroup*)group->r_object, nextAxis);
}

struct Bounds
{
	Point min;
	Point max;

	bool equal(Point lhs, Point rhs) {
		return lhs.x == rhs.x && 
			   lhs.y == rhs.y && 
			   lhs.z == rhs.z;
	}

	bool less(Point lhs, Point rhs) {
		return lhs.x < rhs.x && 
			   lhs.y < rhs.y && 
			   lhs.z < rhs.z;
	}
};

Bounds recalculateBounds(Object* obj)
{
	BVHGroup* asGroup = dynamic_cast<BVHGroup*>(obj);
	if (asGroup == 0x0) {
		Bounds b;
		b.max = obj->getMax();
		b.min = obj->getMin();
		return b;
	}
	else {
		Bounds newBounds, b;
		newBounds.max = Point(-HUGE_VAL);
		newBounds.min = Point(HUGE_VAL);
		auto objects = asGroup->getObjects();
		for (auto groupObj = objects.begin(); groupObj != objects.end(); ++groupObj)
		{
			Bounds temp = recalculateBounds(*groupObj);
			newBounds.min = b.less(newBounds.min, temp.min) ? newBounds.min : temp.min;
			newBounds.max = b.less(newBounds.max, temp.max) ? temp.max : newBounds.max;
		}	

		if (asGroup->l_object != 0x0) {
			Bounds leftBounds = recalculateBounds(asGroup->l_object);
			newBounds.min = b.less(newBounds.min, leftBounds.min) ? newBounds.min : leftBounds.min;
			newBounds.max = b.less(newBounds.max, leftBounds.max) ? leftBounds.max : newBounds.max;
		}
		if (asGroup->r_object != 0x0) {
			Bounds rightBounds = recalculateBounds(asGroup->r_object);
			newBounds.min = b.less(newBounds.min, rightBounds.min) ? newBounds.min : rightBounds.min;
			newBounds.max = b.less(newBounds.max, rightBounds.max) ? rightBounds.max : newBounds.max;
		}

		if (b.equal(newBounds.min, asGroup->getMin()) &&
			b.equal(newBounds.max, asGroup->getMax())) {
		// cool
		}
		else {
			asGroup->setMax(newBounds.max);
			asGroup->setMin(newBounds.min);
		}

		return newBounds;
	}
}

void BVHGroup::preprocess()
{
	splitPointerGroup(this, 0);
	Bounds b = recalculateBounds(this);
	Point min = getMin();
	Point max = getMax();
	if (b.min.x != min.x || b.min.y != min.y || b.min.z != min.z ||
		b.min.x != min.x || b.min.y != min.y || b.min.z != min.z) {
			setMin(b.min);
			setMax(b.max);
	}
}

void BVHGroup::intersect(HitRecord& hit, const RenderContext& context, const Ray& ray) const
{
	// Use this if you're using the group pointers rather then the stl array.
	double firstHit;
	if ((firstHit = intersectBox(ray)) < hit.getMinT()) {
		if (l_object != 0x0) {
			l_object->intersect(hit, context, ray);
		}
		if (r_object != 0x0) {
			r_object->intersect(hit, context, ray);
		}
	}
}

double BVHGroup::intersectBox(const Ray& ray) const
{
	double tmin,tmax,tymin,tymax,tzmin,tzmax;
	double divx,divy,divz;
	Point O = ray.pos;
	Vector V = ray.dir;

	Point corner0 = getMin();
	Point corner1 = getMax();

	divx = 1 / V.x;
	if(divx >= 0) {
		tmin = (corner0.x - O.x) * divx;
		tmax = (corner1.x - O.x) * divx;
	}
	else {
		tmin = (corner1.x - O.x) * divx;
		tmax = (corner0.x - O.x) * divx;
	}

	divy = 1 / V.y;
	if(divy >= 0) {
		tymin = (corner0.y - O.y) * divy;
		tymax = (corner1.y - O.y) * divy;
	}
	else {
		tymin = (corner1.y - O.y) * divy;
		tymax = (corner0.y - O.y) * divy;
	}

	if ( (tmin > tymax) || (tymin > tmax) )
		return HUGE_VAL;
	if (tymin > tmin)
		tmin = tymin;
	if (tymax < tmax)
		tmax = tymax;

	divz = 1 / V.z;
	if(divz >= 0) {
		tzmin = (corner0.z - O.z) * divz;
		tzmax = (corner1.z - O.z) * divz;
	}
	else {
		tzmin = (corner1.z - O.z) * divz;
		tzmax = (corner0.z - O.z) * divz;
	}

	if (tmin > tzmax || tzmin > tmax)
		return HUGE_VAL;
	if (tzmin > tmin)
		tmin = tzmin;
	if (tzmax < tmax)
		tmax = tzmax;

	return tmin < tmax ? tmin : tmax;
}



/*
// This is a bottom up implementation to build the tree.
// Note it does not work completely right
void BVHGroup::addObjectToGroup(Object* object)
{	
	setMin(object->getMin());
	setMax(object->getMax());

	if (l_object == 0x0) {
		l_object = object;
		depth++;
	}
	else if (r_object == 0x0) {
		r_object = object;
	}
	else {
		BVHGroup* left_asGroup = dynamic_cast<BVHGroup*>(l_object);
		BVHGroup* right_asGroup = dynamic_cast<BVHGroup*>(r_object);
		if (left_asGroup == 0x0) {
			Object* temp = l_object;
			l_object = new BVHGroup();
			((BVHGroup*)l_object)->addObjectToGroup(temp);
			((BVHGroup*)l_object)->addObjectToGroup(object);
		}
		else if (right_asGroup == 0x0) {
			Object* temp = r_object;
			r_object = new BVHGroup();
			((BVHGroup*)r_object)->addObjectToGroup(temp);
			((BVHGroup*)r_object)->addObjectToGroup(object);
		}
		else {
			if (left_asGroup->depth <= right_asGroup->depth) {
				left_asGroup->addObjectToGroup(object);
				depth = left_asGroup->depth + 1;
			}
			else {
				right_asGroup->addObjectToGroup(object);
			}
		}
	}
}

void BVHGroup::preprocess()
{
	for each (Object* o in objects) {
		o->preprocess();
		addObjectToGroup(o);
	}

	for each (Object* o in objects) {
		setMin(o->getMin());
		setMax(o->getMax());
	}
}

if ( t < HUGE_VAL )
		{
			Color c(0.6f, 0.5f, 0.3f);
			Color h(1.0f, 1.0f, 1.0f);
			PhongMaterial* mat = new PhongMaterial(c, 0.6, 0.4, h, 80);
			Box* box = new Box(mat, min, max);
			hit.hit(t, box, mat);
		}
*/