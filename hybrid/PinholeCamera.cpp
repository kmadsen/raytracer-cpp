#include "PinholeCamera.h"

PinholeCamera::~PinholeCamera(void)
{
}

void PinholeCamera::preprocess(const RenderContext& context)
{
	Camera::preprocess(context);

	gaze = lookat - eye;
	gaze.normalize();

	u = gaze.cross(up);
	v = u.cross(gaze);
	double ulen = tan(fieldOfView * PI / 360.0);
	u = u / u.length() * ulen; 
	v = v / v.length() * ulen / aspectRatio;
}

void PinholeCamera::makeRay(Ray & ray, const RenderContext& context, const double& x, const double& y) const
{
	double tx, ty;

	// map pixels to 0 to 1
	mapPixel(context, x, y, tx, ty);

	Vector direction = gaze + u * tx + v * ty;
	direction.normalize();
	ray = Ray(eye, direction);
}