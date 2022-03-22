#ifndef DielectricMaterial_h
#define DielectricMaterial_h

#include "Material.h"
#include "Color.h"
#include <math.h>
#include <vector>

class Light;
class RenderContext;
class Point;

class DielectricMaterial : public Material
{
public:
	DielectricMaterial(const double& _eta, const Color& _extinction, const int& _exponent)
		: eta(_eta), extinction(_extinction), exponent(_exponent)
	{}

	virtual ~DielectricMaterial(void);

	MaterialType type() const { return MaterialType::TDielectric; }

	void preprocess(const RenderContext& context);
	void shade(Color& result, const RenderContext& context, const Ray& ray,
		const HitRecord& hit, const Color& atten, int depth) const;
	void hybridShade(Color& result, const Color& colorIn, const RenderContext& context, 
		const Ray& ray, const HitRecord& hit) const;
	Color beersAtten(const double& t) const;
private:
	Color extinction;
	double eta;
	int exponent;
};

#endif // DielectricMaterial_h