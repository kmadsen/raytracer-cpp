#include "PhongMaterial.h"
#include "Primitive.h"
#include "Vector.h"
#include "Light.h"
#include "Scene.h"

PhongMaterial::~PhongMaterial(void)
{
}

void PhongMaterial::preprocess()
{

}

void PhongMaterial::shade(Color& result, const RenderContext& context, const Ray& ray,
						  const HitRecord& hit, const Color& atten, int depth) const
{
	// get the point the ray hit the surface.
	Point hitpos = ray.pos + ray.dir * hit.getMinT();

	Vector normal;
	hit.getPrimitive()->normal(normal, context, hitpos, ray, hit);

	if(normal.dot(ray.dir) > 0) {
		normal = -normal;
	}
	
	const Scene* scene = context.getScene();
	Color allLightsColor = scene->getAmbient()*Ka;
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
			// add diffuse light
			allLightsColor += lightColor * (Kd * cosphi);

			// add specular light
			Vector r = lightDir - ray.dir;
			r.normalize();
			double costheta = normal.dot(r);
			if (costheta > 0)
				specular += lightColor * pow(costheta, exponent);
		}
	}

	result = allLightsColor * color + specular * highlight;
}

