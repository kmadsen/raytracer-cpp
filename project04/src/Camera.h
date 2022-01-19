#ifndef Camera_h
#define Camera_h

class Ray;
class RenderContext;

#include "Point.h"
#include "Vector.h"

class Camera
{
public:
	Camera(void);
	virtual ~Camera(void);

	virtual void preprocess(const RenderContext& context);

	virtual void makeRay(Ray &ray, const RenderContext& context, const double& x, const double& y) const = 0; 
	void mapPixel(const RenderContext& context, const int& ix, const int& iy, double& tx, double& ty) const;

	Point getEye() { return eye; }
	Point getLookat() { return lookat; }
	Vector getUp() { return up; }

protected:
	Point eye;
	Point lookat;
	Vector up;
	int imageWidth;
	int imageHeight;
	double aspectRatio;
};

#endif // Camera_h

