#ifndef SCREEN_H_
#define SCREEN_H_

#include "vec.h"
#include "widget.h"

namespace goatkit {

class ScreenImpl;

class Screen {
private:
	ScreenImpl *scr;

public:
	Screen();
	virtual ~Screen();

	virtual void set_position(float x, float y);
	virtual void set_position(const Vec2 &pos);
	virtual const Vec2 &get_position() const;
	virtual void set_size(float x, float y);
	virtual void set_size(const Vec2 &sz);
	virtual const Vec2 get_size() const;
	virtual const BBox &get_box() const;

	virtual void add_widget(Widget *w);
	virtual int get_widget_count() const;
	virtual Widget *get_widget(int idx) const;
	virtual Widget *get_widget(const char *name) const;

	virtual void show();
	virtual void hide();
	virtual bool is_visible() const;

	virtual void draw() const;

	// window system events used to generate widget events (see event.h)
	void sysev_keyboard(int key, bool press);
	// mouse position as reported by the window system. might fall outside
	void sysev_mouse_button(int bn, bool press, float x, float y);
	void sysev_mouse_motion(float x, float y);
};

}	// namespace goatkit

#endif /* SCREEN_H_ */
