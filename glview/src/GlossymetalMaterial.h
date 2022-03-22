#ifndef GlossymetalMaterial_h
#define GlossymetalMaterial_h

#include "Material.h"
#include "Color.h"

class GlossymetalMaterial : public Material
{
public:
	GlossymetalMaterial(const double& _color, const int& _exponent, const double& _angle)
		: color(_color), exponent(_exponent), angle(_angle) 
	{ }
	virtual ~GlossymetalMaterial(void);

	MaterialType type() const { return MaterialType::TGlossyMetal; }

	void preprocess(const RenderContext& context);
	void shade(Color& result, const RenderContext& context, const Ray& ray,
		const HitRecord& hit, const Color& atten, int depth) const;
	void hybridShade(Color& result, const Color& colorIn, const RenderContext& context, 
		const Ray& ray, const HitRecord& hit) const;
private:
	double color;
	int exponent;
	double angle;
};

#endif 
