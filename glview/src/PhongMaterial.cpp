#include "PhongMaterial.h"
#include "Primitive.h"
#include "Vector.h"
#include "Light.h"
#include "Scene.h"


PhongMaterial::~PhongMaterial(void)
{
}

void PhongMaterial::preprocess(const RenderContext& context)
{
	/*
	Color color;
	double Kd;
	double Ka;
	Color highlight;
	int exponent;
	*/

	Color ambient = context.getScene()->getAmbient();

	mat_specular[0] = highlight.b();
	mat_specular[1] = highlight.g();
	mat_specular[2] = highlight.b();
	mat_specular[3] = 1.0f;

	mat_emission[0] = 0.0f;
	mat_emission[1] = 0.0f;
	mat_emission[2] = 0.0f;
	mat_emission[3] = 0.0f;

	mat_diffuse[0] = Kd * color.r();
	mat_diffuse[1] = Kd * color.g();
	mat_diffuse[2] = Kd * color.b();
	mat_diffuse[3] = 1.0f;

	mat_ambient[0] = Ka * ambient.r();
	mat_ambient[1] = Ka * ambient.g();
	mat_ambient[2] = Ka * ambient.b();
	mat_ambient[3] = 1.0f;

	mat_shininess = exponent;
}

void PhongMaterial::shade(Color& result, const RenderContext& context, const Ray& ray,
						  const HitRecord& hit, const Color& atten, int depth) const
{
	// get the point the ray hit the surface.
	Point hitpos = ray.pos + ray.dir * hit.getMinT();

	Vector normal;
	hit.getPrimitive()->normal(normal, context, hitpos, ray, hit);

	if(normal.dot(ray.dir) > 0)
		normal = -normal;

	const Scene* scene = context.getScene();
	Color allLightsColor = scene->getAmbient()*Ka;
	Color specular;

	int numLights = scene->getLights().size();
	Light** lights = scene->getLightsAsArray();
	for (int i = 0; i < numLights; i++)	{
		Color lightColor;
		Vector lightDir;
		double dist = lights[i]->getLight(lightColor, lightDir, context, hitpos);

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

void PhongMaterial::hybridShade(Color& result, const Color& colorIn, const RenderContext& context, 
		const Ray& ray, const HitRecord& hit) const
{
	result = colorIn;
/*
	if (!renderShadows)
		return;

	// get the point the ray hit the surface.
	Point hitpos = ray.pos + ray.dir * hit.getMinT();

	Vector normal;
	hit.getPrimitive()->normal(normal, context, hitpos, ray, hit);

	if(normal.dot(ray.dir) > 0)
		normal = -normal;

	const Scene* scene = context.getScene();
	Color allLightsColor = scene->getAmbient()*Ka;

	int numLights = scene->getLights().size();
	Light** lights = scene->getLightsAsArray();
	for (int i = 0; i < numLights; i++)	{
		Color lightColor;
		Vector lightDir;
		double dist = lights[i]->getLight(lightColor, lightDir, context, hitpos);

		bool isInShadow = scene->traceShadowRay(context, Ray(hitpos, lightDir), HitRecord(dist));
		double cosphi = normal.dot(lightDir);
		if (isInShadow || cosphi <= 0)
		{
			// add diffuse light
			allLightsColor += lightColor * (Kd * cosphi);

			// add specular light
			Vector r = lightDir - ray.dir;
			r.normalize();
			double costheta = normal.dot(r);
			if (costheta > 0)
				result -= lightColor * pow(costheta, exponent);
		}
	}
	*/
}
