#ifndef Primitive_h
#define Primitive_h

#include "Object.h"

class Material;

class Primitive : public Object
{
public:
	Primitive(void);
	~Primitive(void);

	virtual void preprocess(const RenderContext& context);
	virtual void intersect(HitRecord& hit, const RenderContext& context, const Ray& ray) const;
	virtual void normal(Vector& normal, const RenderContext&, const Point& hitpos,
		const Ray& ray, const HitRecord& hit) const = 0;
	
	void setOpenGLMaterial() const;

	Material* getMaterial() { return material; }
	void setMaterial(Material* _material) { material = _material; } 
protected:
	Material* material;
	Primitive(const Primitive&);
	Primitive& operator=(const Primitive&);
};

#endif // Primitive_h
