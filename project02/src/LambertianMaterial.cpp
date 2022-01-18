#include "LambertianMaterial.h"
#include "Primitive.h"
#include "Vector.h"
#include "Light.h"
#include "Scene.h"

LambertianMaterial::~LambertianMaterial(void)
{
}

void LambertianMaterial::preprocess()
{

}

void LambertianMaterial::shade(Color& result, const RenderContext& context, const Ray& ray,
							   const HitRecord& hit, const Color& atten, int depth) const
{
	// get the point the ray hit the surface.
	Point hitpos = ray.pos + ray.dir * hit.getMinT();

	Vector normal;
	hit.getPrimitive()->normal(normal, context, hitpos, ray, hit);

	if(normal.dot(ray.dir) > 0)
		normal = -normal;

	Scene* scene = context.getScene();
	Color allLightsColor = scene->getAmbient()*Ka;

	auto lights = scene->getLights();
	for (auto light = lights.begin(); light != lights.end(); ++light)
	{
		Color lightColor;
		Vector lightDir;
		double dist = (*light)->getLight(lightColor, lightDir, context, hitpos);

		auto hitRecord = HitRecord(dist);
		bool isInShadow = scene->traceShadowRay(context, Ray(hitpos, lightDir), hitRecord);
		double cosphi = normal.dot(lightDir);
		if(!isInShadow && cosphi > 0)
		{
			allLightsColor += lightColor*(Kd*cosphi);
		}
	}

	result = allLightsColor * color;
}
