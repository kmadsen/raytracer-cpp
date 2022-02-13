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

	int getStartX() const { return startX; }
	int getStartY() const { return startY; }
	int getEndX() const { return endX; }
	int getEndY() const { return endY; }

	void setBounds(int _startX, int _startY, int _endX, int _endY) {
		startX = _startX;
		startY = _startY;
		endX = _endX;
		endY = _endY;
	}

private:
	Scene* scene;
	int startX, startY;
	int endX, endY;
};

#endif // RenderContext_h
