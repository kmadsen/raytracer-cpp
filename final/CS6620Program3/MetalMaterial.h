#ifndef MetalMaterial_h
#define MetalMaterial_h

#include "Material.h"
#include "Color.h"

class MetalMaterial : public Material
{
public:
	MetalMaterial(const Color& _color, const int& _exponent)
		: color(_color), exponent(_exponent) {}
	virtual ~MetalMaterial(void);

	void preprocess();
	void shade(Color& result, const RenderContext& context, const Ray& ray,
		const HitRecord& hit, const Color& atten, int depth) const;

private:
	Color color;
	int exponent;
};

#endif // MetalMaterial_h
