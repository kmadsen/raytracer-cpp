#ifndef PinholeCamera_h
#define PinholeCamera_h

#include "Camera.h"
#include "Point.h"
#include "Vector.h"
#include "Scene.h"
#include "Ray.h"
#include "RenderContext.h"
#include "Constants.h"
#include "Image.h"

class PinholeCamera : public Camera
{
public:
	PinholeCamera(const Point& _eye, const Point& _lookat, const Vector& _up, double hfov)
		: fieldOfView(hfov) 
	{
		eye = _eye;
		lookat = _lookat;
		up = _up;
		up.normalize();
	}

	virtual ~PinholeCamera(void);

	double getFieldOfView() { return fieldOfView; }

	void preprocess(const RenderContext& context);
	void makeRay(Ray & ray, const RenderContext& context, const double& x, const double& y) const;
private:
	double fieldOfView;
	Vector u, v;
	Vector gaze;
};

#endif // PinholeCamera_h
