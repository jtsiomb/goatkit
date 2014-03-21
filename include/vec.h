#ifndef VEC_H_
#define VEC_H_

namespace goatkit {

class Vec2 {
public:
	float x, y;

	Vec2() : x(0), y(0) {}
	Vec2(float xx, float yy) : x(xx), y(yy) {}
};

}	// namespace goatkit

#endif	// VEC_H_
