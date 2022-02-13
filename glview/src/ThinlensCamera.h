#ifndef ThinlensCamera_h
#define ThinlensCamera_h

#include "Camera.h"
#include "Point.h"
#include "Vector.h"
#include "RenderContext.h"

class ThinlensCamera : public Camera
{
public:
	ThinlensCamera(const Point& _eye, const Point& _lookat, const Vector& _up, const double& _hfov, const double& _lensradius, const double& _focus);
	virtual ~ThinlensCamera(void);

	void preprocess(const RenderContext& context);

	void makeRay(Ray & ray, const RenderContext& context, const double& x, const double& y) const;
private:
	double fieldOfView;
	double lensRadius;
	double focus;

	Vector u, v;
	Vector gaze;
	Vector lensU, lensV;
};

#endif // ThinlensCamera_h
