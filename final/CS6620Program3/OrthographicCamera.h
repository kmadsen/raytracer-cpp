#ifndef OrthographicCamera_h
#define OrthographicCamera_h

#include "Camera.h"
#include "Point.h"
#include "Vector.h"

class OrthographicCamera : public Camera
{
public:
	// the eye, lookat and up are trivial. But you need to scale x by some value to describe how big
	// your film is.. So this will behave just like field of view for a pinhole camera.
	// There is another way to describe a camera by u and v vectors.. But it's hard to set up a camera.
	OrthographicCamera(const Point& _eye, const Point& _lookat, const Vector& _up, const double& _scale)
		: scale(_scale)
	{
		eye = _eye;
		lookat = _lookat;
		up = _up;
		up.normalize();
	}

	virtual ~OrthographicCamera(void);

	void makeRay(Ray &ray, const RenderContext& context, 
		const double& x, const double& y) const;
private:
	double scale;
};

#endif OrthographicCamera_h
