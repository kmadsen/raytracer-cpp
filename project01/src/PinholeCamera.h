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
		: eye(_eye), lookat(_lookat), up(_up), fieldOfView(hfov) 
	{
	
	}

	virtual ~PinholeCamera(void);

	void makeRay(Ray & ray, const RenderContext& context, const double& x, const double& y) const
	{
		Vector gaze, u, v, d;
		double aspectRatio;

		/////////// map pixels to 0 to 1
		// TODO handle this somewhere else!
		double tx, ty, iw, ih;
		iw = context.getScene()->getImage()->getXresolution();
		ih = context.getScene()->getImage()->getYresolution();

		tx = -1.0 + ( (1 + 2 * x) / iw );
		ty = -1.0 + ( (1 + 2 * y) / ih );
		/////////// done mapping pixels to 0 to 1.

		aspectRatio = iw / ih;

		gaze = lookat - eye;
		gaze.normalize();

		u = gaze.cross(up);
		v = u.cross(gaze);
		
		double ulen = tan(fieldOfView*PI/360); // tan(theta/2) TODO radians or degrees?
		u = u / u.length() * ulen; 
		
		v = v / v.length() * ulen / aspectRatio;

		d = gaze + u*tx + v*ty;
		d.normalize();
		ray = Ray(eye, d);
	}

private:
	Point eye;
	Point lookat;
	Vector up;
	double fieldOfView;

};

#endif // PinholeCamera_h
