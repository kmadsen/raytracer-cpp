
#ifndef Color_h
#define Color_h

#include <math.h>

class Color {
public:
	Color() 
		: _r(0.0f), _g(0.0f), _b(0.0f) {}

	Color(const float& r, const float& g, const float& b)
		: _r(r), _g(g), _b(b) {}

	virtual ~Color() {
	}

	float r() const { return _r; }
	float g() const { return _g; }
	float b() const { return _b; }

	inline Color(const Color& copy) 
	{
		_r = copy._r;
		_g = copy._g;
		_b = copy._b;
	}

	inline Color& operator=(const Color& copy) 
	{
		_r = copy._r;
		_g = copy._g;
		_b = copy._b;
		return *this;
	}

	inline Color operator+(const Color& c) const 
	{ return Color(_r + c._r, _g+c._g, _b+c._b); }
	inline Color operator+(const float& c) const 
	{ return Color(_r + c, _g+c, _b+c); }

	inline Color& operator+=(const Color& c) {
		_r += c._r; _g += c._g; _b += c._b;
		return *this;
	}
	inline Color& operator+=(const float& c) {
		_r += c; _g += c; _b += c;
		return *this;
	}

	inline Color operator-(const Color& c) const {
		return Color(_r - c._r, _g-c._g, _b-c._b);
	}
	inline Color& operator-=(const Color& c) {
		_r -= c._r; _g -= c._g; _b -= c._b;
		return *this;
	}

	inline Color operator*(const Color& c) const {
		return Color(_r * c._r, _g*c._g, _b*c._b);
	}
	inline Color& operator*=(const Color& c) {
		_r *= c._r; _g *= c._g; _b *= c._b;
		return *this;
	}
	inline Color operator*(float s) const {
		return Color(_r*s, _g*s, _b*s);
	}
	inline Color& operator*=(float s) {
		_r *= s; _g *= s; _b *= s;
		return *this;
	}

	inline Color operator/(float s) const {
		return Color(_r/s, _g/s, _b/s);
	}
	inline Color& operator/=(float s) {
		_r /= s; _g /= s; _b /= s;
		return *this;
	}

	inline Color operator -() const {
		return Color(-_r, -_g, -_b);
	}

	float maxValue() const {
		float max1 = _r > _g ? _r : _g;
		return max1 > _b ? max1 : _b; 
	}

private:
	float _r, _g, _b;
};

#endif