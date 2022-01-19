#include "Camera.h"
#include "RenderContext.h"
#include "Image.h"
#include "Scene.h"


Camera::Camera(void)
{
}

Camera::~Camera(void)
{
}

double dx;
double dy;

void Camera::preprocess(const RenderContext& context)
{
	imageWidth = context.getScene()->getImage()->getXresolution();
	imageHeight = context.getScene()->getImage()->getYresolution();
	aspectRatio = imageWidth / imageHeight;

	dx = 1.0 / imageWidth;
	dy = 1.0 / imageHeight;
}

void Camera::mapPixel(const RenderContext& context, const int& ix, const int& iy, double& tx, double& ty) const
{
	tx = -1.0 + ( (1.0 + 2.0 * ix) / imageWidth );
	ty = -1.0 + ( (1.0 + 2.0 * iy) / imageHeight );

	// add a jitter to remove aliasing
	double rx = dx * (1.0 - 2.0 * ((double)rand() / RAND_MAX));
	double ry = dy * (1.0 - 2.0 * ((double)rand() / RAND_MAX));
	tx += rx;
	ty += ry;
}
