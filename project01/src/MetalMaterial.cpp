#include "MetalMaterial.h"

#include "Scene.h"
#include "RenderContext.h"
#include "Vector.h"
#include "Ray.h"
#include "HitRecord.h"
#include "Primitive.h"
#include "Light.h"

#include <float.h>

MetalMaterial::~MetalMaterial(void)
{
}

void MetalMaterial::preprocess()
{

}

void MetalMaterial::shade(Color& result, const RenderContext& context, const Ray& ray,
						  const HitRecord& hit, const Color& atten, int depth) const
{
	// get the point the ray hit the surface.
	Point hitpos = ray.pos + ray.dir * hit.getMinT();

	Vector normal;
	hit.getPrimitive()->normal(normal, context, hitpos, ray, hit);

	double costheta = normal.dot(ray.dir);
	if(costheta > 0)
		normal = -normal;
	else
		costheta = -costheta;

	const Scene* scene = context.getScene();

	// Accumulate the specular light from every light source.
	Color specular;
	auto lights = scene->getLights();
	for (auto light = lights.begin(); light != lights.end(); ++light)
	{
		Color lightColor;
		Vector lightDir;
		double dist = (*light)->getLight(lightColor, lightDir, context, hitpos);

		auto hitRecord = HitRecord(dist);
		bool isInShadow = scene->traceShadowRay(context, Ray(hitpos, lightDir), hitRecord);
		double cosphi = normal.dot(lightDir);
		if (!isInShadow && cosphi > 0)
		{
			// add specular light
			Vector r = lightDir - ray.dir;
			r.normalize();
			double cosalpha = normal.dot(r);
			if (cosphi > 0)
				specular += lightColor * pow(cosalpha, exponent);
		}
	}

	result = specular * color;

	// Add in reflection
	if (depth < scene->getMaxRayDepth())
	{
		Color reflection;
		Color fresnel = color + (-color + 1.0f) * pow(1-costheta, 5);
		Vector reflectDir = ray.dir + normal * (2 * costheta);
		Ray reflectRay(hitpos, reflectDir);
		auto hitRecord = HitRecord(DBL_MAX);
		scene->traceRay(reflection, context, reflectRay, hitRecord, atten, depth+1);
		result = result + reflection*fresnel;
	}
}

