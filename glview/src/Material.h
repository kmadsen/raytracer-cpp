#ifndef Material_h
#define Material_h

class Color;
class RenderContext;
class Ray;
class HitRecord;

#include "GL/glut.h"

enum MaterialType {
	TPhong,
	TLambertian,
	TDielectric,
	TMetal,
	TGlossyMetal
};
static const char* MaterialName[5] {
	"TPhong",
	"TLambertian",
	"TDielectric",
	"TMetal"
	"TGlossyMetal"
};

class Material
{
public:
	Material(void);
	virtual ~Material(void);

	const char* name() const { return MaterialName[type()]; }
	virtual MaterialType type() const = 0;

	virtual void preprocess(const RenderContext& context) = 0;
	virtual void shade(Color& result, const RenderContext& context, const Ray& ray,
		const HitRecord& hit, const Color& atten, int depth) const = 0;
	virtual void hybridShade(Color& result, const Color& colorIn, const RenderContext& context, 
		const Ray& ray, const HitRecord& hit) const = 0;
	void setOpenGLMaterial() const;
private:
	Material(const Material&);
	Material& operator=(const Material&);
protected:
	// These are for openGL lighting
	float mat_specular[4];
	float mat_emission[4];
	float mat_diffuse[4];
	float mat_ambient[4];
	float mat_shininess;
};

#endif //Material_h