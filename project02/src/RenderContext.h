#ifndef RenderContext_h
#define RenderContext_h

class Scene;

class RenderContext
{
public:
	RenderContext(Scene* _scene)
		: scene(_scene) {}

	virtual ~RenderContext(void);

	Scene* getScene() const { return scene; }

private:
	Scene* scene;
};

#endif // RenderContext_h
