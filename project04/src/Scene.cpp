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
#include "Light.h"
#include "Group.h"
#include <float.h>

#ifdef USING_OPENGL
	#include "GL/glut.h"
#endif 

Scene::Scene(void)
{
}

Scene::~Scene(void)
{
}

void Scene::setImageResolution(int xres, int yres)
{
	if (image != 0x0)
		delete image;
	image = new Image(xres, yres);	
}


void Scene::preprocess()
{
	objects->preprocess();
	camera->preprocess(RenderContext(this));
	for (auto light = lights.begin(); light != lights.end(); ++light) {
		(*light)->preprocess();
	}
}

void Scene::traceRay(Color& result, const RenderContext& context, const Ray& ray,
					 HitRecord& hitRec, const Color& atten, int depth) const
{
	// base case
	float max = atten.maxValue();
	if(depth > maxRayDepth || max < minAtten) 
	{
		result = Color(0.0f, 0.0f, 0.0f); 
	} 
	else
	{
		objects->intersect(hitRec, context, ray);
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
	objects->intersect(hitRec, context, ray);
	return (hitRec.getPrimitive() != 0x0);
}
	

void Scene::renderScene()
{
	int xres, yres, x, y;
	Ray ray;

	xres = image->getXresolution();
	yres = image->getYresolution();

	RenderContext context(this);

	Color atten(1,1,1);
	
	if (samples < 1)
		samples = 1;
	for (x = 0; x < xres; x++) {
		for (y = 0; y < yres; y++) {
			Color totalColor;
			for (int s = 0; s < samples; s++) {
				camera->makeRay(ray, context, x, y);
				HitRecord hitRec(DBL_MAX);
				Color sampleColor;
				traceRay(sampleColor, context, ray, hitRec, atten, 0);
				totalColor += sampleColor;
			}
			totalColor = totalColor / samples;
			image->set(x, y, totalColor);
		}
	}
}
