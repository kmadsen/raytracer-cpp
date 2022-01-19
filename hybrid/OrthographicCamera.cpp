#include "OrthographicCamera.h"

#include "RenderContext.h"
#include "Ray.h"
#include "Scene.h"
#include "Image.h"

OrthographicCamera::~OrthographicCamera(void)
{
}

void OrthographicCamera::makeRay(Ray & ray, const RenderContext& context, const double& x, const double& y) const
{
	/////////// map pixels to 0 to 1
	// TODO handle this somewhere else!
	double tx, ty, iw, ih;
	iw = context.getScene()->getImage()->getXresolution();
	ih = context.getScene()->getImage()->getYresolution();

	tx = -1.0 + ( (1 + 2 * x) / iw );
	ty = -1.0 + ( (1 + 2 * y) / ih );
	/////////// done mapping pixels to 0 to 1.

	double aspectRatio = iw / ih;

	ray.dir = lookat - eye;
	ray.dir.normalize();

	Vector u = ray.dir.cross(up);
	u.normalize();

	// TODO make this dumb math correct damnit!
	Vector posAsVec = eye.asVector() + (u*tx + up*ty)*scale;
	ray.pos.x = posAsVec.x;
	ray.pos.y = posAsVec.y;
	ray.pos.z = posAsVec.z;
}
