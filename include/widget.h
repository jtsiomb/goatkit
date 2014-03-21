#ifndef GOATKIT_WIDGET_H_
#define GOATKIT_WIDGET_H_

#include "vec.h"
#include "event.h"

namespace goatkit {

struct BBox {
	Vec2 bmin, bmax;
};

struct WidgetImpl;

class Widget {
private:
	WidgetImpl *widget;

public:
	Widget();
	virtual ~Widget();

	virtual const char *get_type_name() const;

	virtual void show();
	virtual void hide();
	virtual float get_visibility() const;
	virtual bool is_visible() const;

	virtual void activate();
	virtual void deactivate();
	virtual float get_active() const;
	virtual bool is_active() const;

	virtual void press();
	virtual void release();
	virtual float get_pressed() const;
	virtual bool is_pressed() const;

	virtual void mousein();
	virtual void mouseout();
	virtual float get_under_mouse() const;
	virtual bool is_under_mouse() const;

	virtual void set_position(float x, float y);
	virtual void set_position(const Vec2 &pos);
	virtual const Vec2 &get_position() const;

	virtual void set_size(float x, float y);
	virtual void set_size(const Vec2 &size);
	virtual const Vec2 get_size() const;

	virtual const BBox &get_box() const;

	virtual bool hit_test(const Vec2 &pt) const;

	virtual void draw() const;

	// low level events
	virtual void on_mouse_button(const ButtonEvent &ev);
	virtual void on_mouse_motion(const MotionEvent &ev);
	virtual void on_mouse_focus(const FocusEvent &ev);
	virtual void on_key(const KeyEvent &ev);

	// high level events
	virtual void on_click();
	virtual void on_double_click();
	virtual void on_change();
	//virtual void on_drag_move(int bn, const Vec2 &pt);
	//virtual void on_drag_release(int bn, const Vec2 &pt);

	// event dispatcher
	virtual void handle_event(const Event &ev);
};

}

#endif	// GOATKIT_WIDGET_H_
