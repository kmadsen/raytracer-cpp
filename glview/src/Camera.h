#ifndef Camera_h
#define Camera_h

class Ray;
class RenderContext;

#include "Point.h"
#include "Vector.h"

enum CameraType { TPinhole, TOrtho, TThinlens };
static const char* CameraName[3] { "TPinhole", "TOrtho", "TThinlens" };

class Camera
{
public:
	Camera(void);
	virtual ~Camera(void);

	const char* name() const { return CameraName[type()]; }
	virtual CameraType type() const = 0;

	virtual void preprocess(const RenderContext& context);
	virtual void makeRay(Ray &ray, const RenderContext& context, const double& x, const double& y) const = 0; 
	void mapPixel(const RenderContext& context, const int& ix, const int& iy, double& tx, double& ty) const;

	Point getEye() const { return eye; }
	Point getLookat() const { return lookat; }
	Vector getUp() const { return up; }

	void setEye(const Point& _eye) { eye = _eye; }
	void setLookat(const Point& _lookat) { lookat = _lookat; }

protected:
	Point eye;
	Point lookat;
	Vector up;
	int imageWidth;
	int imageHeight;
	double aspectRatio;
};

#endif // Camera_h
