#ifndef LambertianMaterial_h
#define LambertianMaterial_h

#include "Material.h"
#include "Color.h"

class LambertianMaterial : public Material
{
public:
	LambertianMaterial(const Color& _color, const double& _Kd, const double& _Ka)
		: color(_color), Kd(_Kd), Ka(_Ka) {}
	virtual ~LambertianMaterial(void);

	void preprocess(const RenderContext& context);
	void shade(Color& result, const RenderContext& context, const Ray& ray,
		const HitRecord& hit, const Color& atten, int depth) const;
	void hybridShade(Color& result, const Color& colorIn, const RenderContext& context, 
		const Ray& ray, const HitRecord& hit) const;
private:
	Color color;
	double Kd;
	double Ka;
};

#endif // LambertianMaterial_h