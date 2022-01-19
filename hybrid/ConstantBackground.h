#ifndef ConstantBackground_h
#define ConstantBackground_h

#include "Color.h"
#include "Background.h"

class ConstantBackground : public Background
{
public:
	ConstantBackground(const Color& _color)
		: color(_color) {}
	virtual ~ConstantBackground(void);

	Color getBackground() const { 
		return color; 
	}
private:
	Color color;
};

#endif // ConstantBackground_h