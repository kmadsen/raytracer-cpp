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

void MetalMaterial::preprocess(const RenderContext& context)
{
	// TODO
	/*
	Color color;
	int exponent;
	*/

	Color ambient = context.getScene()->getAmbient();

	mat_specular[0] = color.r();
	mat_specular[1] = color.g();
	mat_specular[2] = color.b();
	mat_specular[3] = 1.0f;

	mat_emission[0] = 0.0f;
	mat_emission[1] = 0.0f;
	mat_emission[2] = 0.0f;
	mat_emission[3] = 0.0f;

	mat_diffuse[0] = 0.0f;
	mat_diffuse[1] = 0.0f;
	mat_diffuse[2] = 0.0f;
	mat_diffuse[3] = 0.0f;

	mat_ambient[0] = ambient.r();
	mat_ambient[1] = ambient.g();
	mat_ambient[2] = ambient.b();
	mat_ambient[3] = 1.0f;

	mat_shininess = exponent;
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
	int numLights = scene->getLights().size();
	Light** lights = scene->getLightsAsArray();
	for (int i = 0; i < numLights; i++)	{
		Color lightColor;
		Vector lightDir;
		double dist = lights[i]->getLight(lightColor, lightDir, context, hitpos);

		bool isInShadow = scene->traceShadowRay(context, Ray(hitpos, lightDir), HitRecord(dist));
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
		scene->traceRay(reflection, context, reflectRay, HitRecord(DBL_MAX), atten, depth+1);
		result = result + reflection*fresnel;
	}
}

void MetalMaterial::hybridShade(Color& result, const Color& colorIn, const RenderContext& context, 
		const Ray& ray, const HitRecord& hit) const
{
	result = colorIn;


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
/*
	if (!renderShadows) {
		// Accumulate the specular light from every light source.
		Color specular;
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
				// add specular light
				Vector r = lightDir - ray.dir;
				r.normalize();
				double cosalpha = normal.dot(r);
				if (cosphi > 0)
					result -= lightColor * pow(cosalpha, exponent);
			}
		}
	}*/

	Color reflection;
	Color fresnel = color + (-color + 1.0f) * pow(1-costheta, 5);
	Vector reflectDir = ray.dir + normal * (2 * costheta);
	Ray reflectRay(hitpos, reflectDir);
	scene->traceRay(reflection, context, reflectRay, HitRecord(DBL_MAX), Color(1.0f, 1.0f, 1.0f), 1);
	result = result + reflection*fresnel;
}



