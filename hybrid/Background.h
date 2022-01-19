#ifndef Background_h
#define Background_h

class Color;

class Background
{
public:
	Background(void);
	virtual ~Background(void);

	virtual Color getBackground() const = 0;
};

#endif // Background_h
