#include "DielectricMaterial.h"
#include "Primitive.h"
#include "Vector.h"
#include "Light.h"
#include "Scene.h"
#include "RenderContext.h"
#include <float.h>


DielectricMaterial::~DielectricMaterial(void)
{
}

void DielectricMaterial::preprocess(const RenderContext& context)
{
	// TODO
	/*
	Color extinction;
	double eta;
	int exponent;
	*/

	Color ambient = context.getScene()->getAmbient();

	mat_specular[0] = 1.0f;
	mat_specular[1] = 1.0f;
	mat_specular[2] = 1.0f;
	mat_specular[3] = 1.0f;

	mat_emission[0] = 0.0f;
	mat_emission[1] = 0.0f;
	mat_emission[2] = 0.0f;
	mat_emission[3] = 0.0f;

	mat_diffuse[0] = 0.0f;
	mat_diffuse[1] = 0.0f;
	mat_diffuse[2] = 0.0f;
	mat_diffuse[3] = 0.0f;

//	mat_ambient[0] = ambient.r();
//	mat_ambient[1] = ambient.g();
//	mat_ambient[2] = ambient.b();
//	mat_ambient[3] = 1.0f;

	mat_shininess = exponent;
}

void DielectricMaterial::shade(Color& result, const RenderContext& context, const Ray& ray,
							   const HitRecord& hit, const Color& atten, int depth) const
{
	// get the point the ray hit the surface.
	Point hitpos = ray.pos + ray.dir * hit.getMinT();

	Vector normal;
	hit.getPrimitive()->normal(normal, context, hitpos, ray, hit);

	double costheta = normal.dot(ray.dir);
	Vector unflippedNormal = normal;
	
	bool rayEntering = false;
	double etatmp;
	if (costheta > 0) 
	{
		// flip the normal if it is in the same direction as the ray.
		// etatmp = 1/eta because I'm assuming the air's eta is 1.
		etatmp = 1/eta;
		normal = -normal;
	}
	else
	{
		rayEntering = true;
		etatmp = eta;
		costheta = -costheta;
	}

	// get specular highlight for every light
	const Scene* scene = context.getScene();
	Color specular(0.0f,0.0f,0.0f);
	auto lights = scene->getLights();
	for (auto light = lights.begin(); light != lights.end(); ++light)
	{
		Color lightColor;
		Vector lightDir;
		double dist = (*light)->getLight(lightColor, lightDir, context, hitpos);

		auto hitRecord = HitRecord(dist);
		bool isInShadow = scene->traceShadowRay(context, Ray(hitpos, lightDir), hitRecord);
		double cosphi = unflippedNormal.dot(lightDir);
		if (!isInShadow && cosphi > 0)
		{
			// add phong specular highlight
			Vector halfvec;
			if ( rayEntering )
				halfvec = Vector( lightDir - ray.dir );
			else
				halfvec = Vector( lightDir + ray.dir );
			halfvec.normalize();

			double NdotH = unflippedNormal.dot(halfvec);
			if ( NdotH > 0 ) {
				double powterm = pow( NdotH, exponent );
				specular += lightColor * powterm;
			}
		}
	}

	// add in the specular highlight
	result = specular * Color(1.0f, 1.0f, 1.0f);

	// refraction ray
	if(depth > scene->getMaxRayDepth() || atten.maxValue() < scene->getMinAttenuation())
	{
		//prune ray tree by stopping here
		result = Color(0,0,0);
		return;
	}
	else
	{
		double costheta2 = 1.0 + (costheta * costheta - 1.0)/(etatmp*etatmp); // costheta2 squared

		if (costheta2 < 0)
		{
			//total internal reflection
			Color reflectColor;
			Vector reflectDir = ray.dir + (normal * 2 * costheta);
			HitRecord reflectHit(DBL_MAX);
			scene->traceRay(reflectColor, context, Ray(hitpos, reflectDir), 
				reflectHit, atten, depth + 1);
			if (rayEntering)
				result += reflectColor;
			else
				result += reflectColor * beersAtten(reflectHit.getMinT());
		}
		else
		{
			/*
			 * trace both transmission and reflection rays
			 */

			// First calculate the stuff needed.
			costheta2 = sqrt(costheta2);
			double cosm = costheta < costheta2 ? costheta : costheta2; // minimum
			double R0 = (etatmp - 1.0)/(etatmp + 1.0); 
			R0 = R0 * R0;  //Not sure if this should be squared
			double Fr = R0  + ( 1.0 - R0) * pow((1.0 - cosm),5);
			
			//Trace Reflection Ray
			Color reflectColor;
			Vector reflectDir = ray.dir + (normal * 2 * costheta);
			HitRecord reflectHit(DBL_MAX);
			scene->traceRay(reflectColor, context, Ray(hitpos, reflectDir), reflectHit, atten * Fr, depth + 1);
			
			if (rayEntering)
				result += reflectColor * Fr;
			else
				result += reflectColor * Fr * beersAtten(reflectHit.getMinT());
			
			// Trace Transmission Ray
			double Ft = 1.0 - Fr;
			Color transColor;
			Vector transDir = ray.dir * (1.0/etatmp) + normal * ((costheta/etatmp) - costheta2);
			HitRecord transHit(DBL_MAX);
			scene->traceRay(transColor, context, Ray(hitpos, transDir), transHit, atten * Ft, depth + 1);
			
			if (rayEntering)
				result += transColor * Ft * beersAtten(transHit.getMinT());
			else
				result += transColor * Ft;
		}
	}
}

Color DielectricMaterial::beersAtten(const double& t) const {
	double Ar = log((double)extinction.r());
	double Ag = log((double)extinction.g());
    double Ab = log((double)extinction.b());
	Color c(exp(t * Ar), exp(t * Ag), exp(t * Ab));
	return c;
}

void DielectricMaterial::hybridShade(Color& result, const Color& colorIn, const RenderContext& context, 
		const Ray& ray, const HitRecord& hit) const
{
	// get initial values
	result = colorIn;
	Color atten(1.0f, 1.0f, 1.0f);
	int depth = 1;
	Scene* scene = context.getScene();

	// get the point the ray hit the surface.
	Point hitpos = ray.pos + ray.dir * hit.getMinT();

	Vector normal;
	hit.getPrimitive()->normal(normal, context, hitpos, ray, hit);

	double costheta = normal.dot(ray.dir);
	Vector unflippedNormal = normal;
	
	bool rayEntering = false;
	double etatmp;
	if (costheta > 0) 
	{
		// flip the normal if it is in the same direction as the ray.
		// etatmp = 1/eta because I'm assuming the air's eta is 1.
		etatmp = 1/eta;
		normal = -normal;
	}
	else
	{
		rayEntering = true;
		etatmp = eta;
		costheta = -costheta;
	}

	double costheta2 = 1.0 + (costheta * costheta - 1.0)/(etatmp*etatmp); // costheta2 squared

	if (costheta2 < 0)
	{
		//total internal reflection
		Color reflectColor;
		Vector reflectDir = ray.dir + (normal * 2 * costheta);
		HitRecord reflectHit(DBL_MAX);
		scene->traceRay(reflectColor, context, Ray(hitpos, reflectDir), 
			reflectHit, atten, depth + 1);
		if (rayEntering)
			result += reflectColor;
		else
			result += reflectColor * beersAtten(reflectHit.getMinT());
	}
	else
	{
		/*
		* trace both transmission and reflection rays
		*/

		// First calculate the stuff needed.
		costheta2 = sqrt(costheta2);
		double cosm = costheta < costheta2 ? costheta : costheta2; // minimum
		double R0 = (etatmp - 1.0)/(etatmp + 1.0); 
		R0 = R0 * R0;  //Not sure if this should be squared
		double Fr = R0  + ( 1.0 - R0) * pow((1.0 - cosm),5);

		//Trace Reflection Ray
		Color reflectColor;
		Vector reflectDir = ray.dir + (normal * 2 * costheta);
		HitRecord reflectHit(DBL_MAX);
		scene->traceRay(reflectColor, context, Ray(hitpos, reflectDir), reflectHit, atten * Fr, depth + 1);

		if (rayEntering)
			result += reflectColor * Fr;
		else
			result += reflectColor * Fr * beersAtten(reflectHit.getMinT());

		// Trace Transmission Ray
		double Ft = 1.0 - Fr;
		Color transColor;
		Vector transDir = ray.dir * (1.0/etatmp) + normal * ((costheta/etatmp) - costheta2);
		HitRecord transHit(DBL_MAX);
		scene->traceRay(transColor, context, Ray(hitpos, transDir), transHit, atten * Ft, depth + 1);

		if (rayEntering)
			result += transColor * Ft * beersAtten(transHit.getMinT());
		else
			result += transColor * Ft;
	}
}

