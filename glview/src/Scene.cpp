#include "Scene.h"
#include "HitRecord.h"
#include "RenderContext.h"
#include "Ray.h"
#include "Image.h"
#include "Camera.h"
#include "Object.h"
#include "Color.h"
#include "Background.h"
#include "Material.h"
#include "Light.h"
#include "Group.h"
#include <float.h>

#include <iomanip>
#include <iostream>
#include <fstream>
#include <time.h>

#include "GL/glut.h"

Scene::Scene(void)
{
}

Scene::~Scene(void)
{
	//Light[] lightsAsArray;
	delete[] stencils;
	delete[] pixels;
	delete background;
	delete image;
	delete camera;
	for (auto light : lights) {
		delete light;
	}
	lights.clear();
	delete group;
}

void Scene::setImageResolution(int xres, int yres)
{
	if (image != nullptr) {
		delete image;
	}
	image = new Image(xres, yres);	
}


void Scene::preprocess()
{
	RenderContext context(this);
	group->preprocess(context);
	camera->preprocess(RenderContext(this));
	for (auto light : lights) {
		light->preprocess(context);
	}

	stencils = new int[image->getXresolution() * image->getYresolution()];
	pixels = new float[3 * image->getXresolution() * image->getYresolution()];
}

void Scene::traceRay(Color& result, const RenderContext& context, const Ray& ray,
					 HitRecord& hitRec, const Color& atten, int depth) const
{
	// base case
	float max = atten.maxValue();
	if(depth > maxRayDepth || max < minAtten) 
	{
		result = Color(0.0f, 0.0f, 0.0f); 
	} 
	else
	{
		group->intersect(hitRec, context, ray);
		if (hitRec.getMinT() < DBL_MAX) {						
			hitRec.getMaterial()->shade(result, context, ray, hitRec, atten, depth + 1);
		}
		else {
			result = background->getBackground(); 
		}
	}
}

bool Scene::traceShadowRay(const RenderContext& context, const Ray& ray, HitRecord& hitRec) const
{
	group->intersect(hitRec, context, ray);
	return (hitRec.getPrimitive() != 0x0);
}
	

void Scene::renderScene()
{
	int xres, yres, x, y;
	Ray ray;

	xres = image->getXresolution();
	yres = image->getYresolution();

	RenderContext context(this);
	Color atten(1,1,1);
	
	if (samples < 1) {
		samples = 1;
	}
	for (x = 0; x < xres; x++) {
		for (y = 0; y < yres; y++) {
			Color totalColor;
			for (int s = 0; s < samples; s++) {
				camera->makeRay(ray, context, x, y);
				HitRecord hitRec(DBL_MAX);
				Color sampleColor;
				traceRay(sampleColor, context, ray, hitRec, atten, 0);
				totalColor += sampleColor;
			}
			totalColor = totalColor / samples;
			image->set(x, y, totalColor);
		}
	}
}

void Scene::setUpLightsOpenGL()
{
	glEnable(GL_LIGHTING);
	
	GLenum lightEnum = GL_LIGHT0;
	GLfloat light_position[4];
	GLfloat light_color[4];

	for (auto light : lights) {
		Color lightColor;
		Vector lightDirection;
		Point lightPosition;
		light->getLightInfo(lightColor, lightDirection, lightPosition);  

		glEnable(lightEnum);

		// add point light
		light_position[0] = lightPosition.x;
		light_position[1] = lightPosition.y;
		light_position[2] = lightPosition.z;
		light_position[3] = 1.0;

		light_color[0] = lightColor.r();
		light_color[1] = lightColor.g();
		light_color[2] = lightColor.b();
		light_color[3] = 1.0;

		glLightfv(lightEnum, GL_DIFFUSE, light_color);
		glLightfv(lightEnum, GL_SPECULAR, light_color);
		glLightfv(lightEnum, GL_POSITION, light_position);

		lightEnum++;
	}

	glEnable(GL_DEPTH_TEST);
}

void Scene::renderOpenGL(const Vector& lookdir)
{
	// First Pass, render to the stencil buffer
	glEnable(GL_STENCIL_TEST);  

	group->rasterize(lookdir);	

	glReadPixels(0, 0, 512, 512, GL_STENCIL_INDEX, GL_INT, stencils);
	glDisable(GL_STENCIL_TEST);

	glReadPixels(0, 0, 512, 512, GL_RGB, GL_FLOAT, pixels); 
}

Color getColorFromArray(float *pixels, int x, int y)
{
	int bufferIndex = x * y;
	return Color(pixels[bufferIndex * 3],
		pixels[bufferIndex * 3 + 1],
		pixels[bufferIndex * 3 + 2]);
}

int getStencilFromArray(const int *stencils, int x, int y)
{
	int bufferIndex = y * 512 + x;
	return stencils[bufferIndex];
}

void Scene::renderRasterizedScene(const RenderContext& context)
{
	int xsrt = context.getStartX();
	int ysrt = context.getStartY();
	int xend = context.getEndX();
	int yend = context.getEndY();
	int imageXRes = context.getScene()->getImage()->getXresolution();

	Ray ray;

	Color atten(1,1,1);

	if (samples < 1) {
		samples = 1;
	}
	auto objects = group->getObjects();
	for (int y = ysrt; y < yend; y++) {
		for (int x = xsrt; x < xend; x++) {
			Color sampleColor = background->getBackground(); 
			camera->makeRay(ray, context, x, y);
			HitRecord hitRec(DBL_MAX);

			int bufferIndex = y * imageXRes + x;

			int objectIndex = stencils[bufferIndex];

			if (objectIndex >= 0) {
				objects[objectIndex]->intersect(hitRec, context, ray);
				if (hitRec.getMinT() < DBL_MAX) {	
					Color rastColor(pixels[bufferIndex * 3],
						pixels[bufferIndex * 3 + 1],
						pixels[bufferIndex * 3 + 2]);

					//hitRec.getMaterial()->shade(sampleColor, context, ray, hitRec, Color(1.0f, 1.0f, 1.0f), 1);
					hitRec.getMaterial()->hybridShade(sampleColor, rastColor, context, ray, hitRec);

					//pixels[bufferIndex * 3 + 0] = sampleColor.r();
					//pixels[bufferIndex * 3 + 1] = sampleColor.g();
					//pixels[bufferIndex * 3 + 2] = sampleColor.b();
				}
			} 
			//objects->intersect(hitRec, context, ray);
			//if (hitRec.getMinT() < DBL_MAX) {						
			//	hitRec.getMaterial()->shade(sampleColor, context, ray, hitRec, atten, 1);
			//}

			image->set(x, y, sampleColor);
			bufferIndex ++;
		}
	}
	//glDrawPixels(512, 512, GL_RGB, GL_FLOAT, pixels); 
}

/*
void Scene::antialiasImage(const int *stencils)
{
	// anti alias
	for (y = 1; y < yres-1; y++) {
		for (x = 1; x < xres-1; x++) {			
			int numEdges = 0;
			if (getStencilFromArray(stencils, x, y) != getStencilFromArray(stencils, x-1, y)) {
				numEdges++;
			}
			if (getStencilFromArray(stencils, x, y) != getStencilFromArray(stencils, x+1, y)) {
				numEdges++;
			}
			if (getStencilFromArray(stencils, x, y) != getStencilFromArray(stencils, x, y-1)) {
				numEdges++;
			}
			if (getStencilFromArray(stencils, x, y) != getStencilFromArray(stencils, x, y+1)) {
				numEdges++;
			}

			if (numEdges > 0) {
				for (int i = 0; i < numEdges; i++) {
					camera->makeRay(ray, context, x, y);
					HitRecord hitRec(DBL_MAX);
					traceRay(sampleColor, context, ray 
				}

				accum = accum / 4.0f;
				image->set(x,y, xyColor);
			}
		}
	}
	*/