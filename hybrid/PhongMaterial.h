#ifndef PhongMaterial_h
#define PhongMaterial_h

#include "Material.h"
#include "Color.h"

class Light;
class Vector;
class Object;

class PhongMaterial : public Material
{
public:
	PhongMaterial(const Color& _color, const double& _Kd, const double& _Ka, 
				  const Color& _highlight, const int& _exponent)
				  : color(_color), Kd(_Kd), Ka(_Ka),
					highlight(_highlight), exponent(_exponent) {}
	virtual ~PhongMaterial(void);

	void preprocess(const RenderContext& context);
	void shade(Color& result, const RenderContext& context, const Ray& ray,
		const HitRecord& hit, const Color& atten, int depth) const;
	void hybridShade(Color& result, const Color& colorIn, const RenderContext& context, 
		const Ray& ray, const HitRecord& hit) const;
private:
	void shadeLight(Color& result, Light* light, const Object* world, 
		const Ray& viewRay, const RenderContext& context, const Vector& surfaceNormal) const;

	Color color;
	double Kd;
	double Ka;
	Color highlight;
	int exponent;
};

#endif // PhongMaterial_h
