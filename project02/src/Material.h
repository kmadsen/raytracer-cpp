#ifndef Material_h
#define Material_h

class Color;
class RenderContext;
class Ray;
class HitRecord;

class Material
{
public:
	Material(void);
	virtual ~Material(void);

	virtual void preprocess() = 0;
	virtual void shade(Color& result, const RenderContext& context, const Ray& ray,
		const HitRecord& hit, const Color& atten, int depth) const = 0;
 private:
  Material(const Material&);
  Material& operator=(const Material&);
};

#endif //Material_h