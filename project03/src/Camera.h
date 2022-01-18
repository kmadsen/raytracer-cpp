#ifndef Camera_h
#define Camera_h

class Ray;
class RenderContext;

class Camera
{
public:
	Camera(void);
	virtual ~Camera(void);

	//virtual void preprocess(double aspect_ratio) = 0;
	virtual void makeRay(Ray &ray, const RenderContext& context, const double& x, const double& y) const = 0; 
};

#endif // Camera_h

