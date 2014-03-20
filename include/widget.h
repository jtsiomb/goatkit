#ifndef GAMEUI_WIDGET_H_
#define GAMEUI_WIDGET_H_

namespace gameui {

class Vec2 {
public:
	float x, y;

	Vec2() : x(0), y(0) {}
	Vec2(float xx, float yy) : x(xx), y(yy) {}
};

class BBox {
public:
	Vec2 bmin, bmax;
};

struct WidgetImpl;

class Widget {
private:
	WidgetImpl *impl;

public:
	enum VisState {
		VST_HIDDEN,
		VST_EASEIN,
		VST_VISIBLE,
		VST_EASEOUT
	};
	enum ActiveState {
		AST_INACTIVE,
		AST_EASEIN,
		AST_ACTIVE,
		AST_EASEOUT
	};

	Widget();
	virtual ~Widget();

	virtual void show();
	virtual void hide();
	virtual float get_visibility() const;

	virtual void activate();
	virtual void deactivate();
	virtual float get_active() const;

	virtual const BBox &get_box() const;
	virtual const Vec2 &get_position() const;
	virtual const Vec2 &get_size() const;

	virtual bool hit_test(const Vec2 &pt) const;

	virtual void draw() const = 0;
};

long get_cur_time();

}

#endif	// GAMEUI_WIDGET_H_
