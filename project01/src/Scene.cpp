#include "Scene.h"
#include "HitRecord.h"
#include "RenderContext.h"
#include "Ray.h"
#include "Image.h"
#include "Camera.h"
#include "Object.h"
#include "Color.h"
#include "Background.h"
#include "Material.h"
#include <float.h>

Scene::Scene(void)
{
}

Scene::~Scene(void)
{
}

void Scene::preprocess()
{

}

void Scene::traceRay(Color& result, const RenderContext& context, const Ray& ray,
					 HitRecord& hitRec, const Color& atten, int depth) const
{
	// base case
	float max = atten.maxValue();
	if(depth >= maxRayDepth || max < minAtten) 
	{
		result = Color(0.0f, 0.0f, 0.0f); 
	} 
	else
	{
		object->intersect(hitRec, context, ray);
		if (hitRec.getMinT() < DBL_MAX) {						
			hitRec.getMaterial()->shade(result, context, ray, hitRec, atten, depth + 1);
		}
		else {
			result = background->getBackground(); 
		}
	}
}

bool Scene::traceShadowRay(const RenderContext& context, const Ray& ray, HitRecord& hitRec) const
{
	object->intersect(hitRec, context, ray);
	return (hitRec.getPrimitive() != 0x0);
}
	

void Scene::renderScene()
{
	int xres, yres, x, y;
	Ray ray;
	Color color;

	xres = image->getXresolution();
	yres = image->getYresolution();

	RenderContext context(this);

	Color atten(1,1,1);

	for (x = 0; x < xres; x++) {
		for (y = 0; y < yres; y++) {
			camera->makeRay(ray, context, x, y);
			HitRecord hitRec(DBL_MAX);
			traceRay(color, context, ray, hitRec, atten, 0);
			image->set(x, y, color);
		}
	}
}

