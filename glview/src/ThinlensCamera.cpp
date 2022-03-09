#include "ThinlensCamera.h"
#include "Ray.h"
#include "Vector.h"
#include "Point.h"
#include "Constants.h"
#include <cstdlib>
#include <ctime> 

ThinlensCamera::ThinlensCamera(const Point& _eye, const Point& _lookat, const Vector& _up, const double& _hfov, const double& _lensradius, const double& _focus)
: fieldOfView(_hfov), lensRadius(_lensradius), focus(_focus) 
{
	eye = _eye;
	lookat = _lookat;
	up = _up;
	up.normalize();
}

ThinlensCamera::~ThinlensCamera(void)
{
}

void ThinlensCamera::preprocess(const RenderContext& context)
{
	Camera::preprocess(context);

	// calculate u and v
	gaze = lookat-eye;
	gaze.normalize();
	u = gaze.cross(up);
	v = u.cross(gaze);

	// scale u
	double ulen = tan(fieldOfView/2.0 * PI / 180.0);
	lensU = u * lensRadius;
	u.normalize();
	u = u * ulen;

	// scale v
	double vlen = ulen/aspectRatio;
	lensV = v * lensRadius;
	v.normalize(); 
	v = v * vlen;
}

void ThinlensCamera::makeRay(Ray & ray, const RenderContext& context, const double& x, const double& y) const
{
	// get mapped pixels from -1.0 to 1.0
	double tx, ty;
	mapPixel(context, x, y, tx, ty);

	double randomU = 1.0;
	double randomV = 1.0;
	while(sqrt(randomU * randomU + randomV * randomV) > 1)
	{
		randomU = (1.0 - 2.0 * ((double)rand() / RAND_MAX));
		randomV = (1.0 - 2.0 * ((double)rand() / RAND_MAX));
	}

	Vector offset = lensU * randomU + lensV * randomV;
	Vector direction = gaze + u * tx + v * ty;
	direction.normalize();
	direction = (direction * focus) - offset;
	direction.normalize();
	ray = Ray(eye + offset, direction);
}