#include <stdio.h>
#include <math.h>
#include <string>
#include <sstream>
#include "widget.h"
#include "boolanm.h"
#include "theme.h"

namespace goatkit {

struct WidgetImpl {
	std::string name, text;
	BBox box;

	BoolAnim visible, active, press, hover;
};


Widget::Widget()
{
	static int widget_count;

	widget = new WidgetImpl;

	std::stringstream sstr;
	sstr << get_type_name() << widget_count++;
	widget->name = sstr.str();

	widget->box.bmin = Vec2(0, 0);
	widget->box.bmax = Vec2(1, 1);

	widget->visible.set(true);
	widget->active.set(true);

	widget->hover.set_transition_duration(250);
	widget->press.set_transition_duration(50);
}

Widget::~Widget()
{
	delete widget;
}

const char *Widget::get_type_name() const
{
	return "widget";
}

void Widget::set_name(const char *name)
{
	widget->name = std::string(name);
}

const char *Widget::get_name() const
{
	return widget->name.c_str();
}

void Widget::set_text(const char *text)
{
	widget->text = std::string(text);
}

const char *Widget::get_text() const
{
	return widget->text.c_str();
}

void Widget::show()
{
	widget->visible.change(true);
}

void Widget::hide()
{
	widget->visible.change(false);
}

float Widget::get_visibility() const
{
	return widget->visible.get_value();
}

bool Widget::is_visible() const
{
	return widget->visible.get_state();
}

void Widget::activate()
{
	widget->active.change(true);
}

void Widget::deactivate()
{
	widget->active.change(false);
}

float Widget::get_active() const
{
	return widget->active.get_value();
}

bool Widget::is_active() const
{
	return widget->active.get_state();
}

void Widget::press()
{
	widget->press.change(true);
}

void Widget::release()
{
	widget->press.change(false);
}

float Widget::get_pressed() const
{
	return widget->press.get_value();
}

bool Widget::is_pressed() const
{
	return widget->press.get_state();
}

void Widget::mousein()
{
	widget->hover.change(true);
}

void Widget::mouseout()
{
	widget->hover.change(false);
}

float Widget::get_under_mouse() const
{
	return widget->hover.get_value();
}

bool Widget::is_under_mouse() const
{
	return widget->hover.get_state();
}

void Widget::set_position(float x, float y)
{
	set_position(Vec2(x, y));
}

void Widget::set_position(const Vec2 &pos)
{
	Vec2 sz = get_size();

	widget->box.bmin = pos;
	widget->box.bmax.x = pos.x + sz.x;
	widget->box.bmax.y = pos.y + sz.y;
}

const Vec2 &Widget::get_position() const
{
	return widget->box.bmin;
}

void Widget::set_size(float x, float y)
{
	set_size(Vec2(x, y));
}

void Widget::set_size(const Vec2 &sz)
{
	widget->box.bmax.x = widget->box.bmin.x + sz.x;
	widget->box.bmax.y = widget->box.bmin.y + sz.y;
}

const Vec2 Widget::get_size() const
{
	return Vec2(widget->box.bmax.x - widget->box.bmin.x,
			widget->box.bmax.y - widget->box.bmin.y);
}


const BBox &Widget::get_box() const
{
	return widget->box;
}

bool Widget::hit_test(const Vec2 &pt) const
{
	return pt.x >= widget->box.bmin.x && pt.x < widget->box.bmax.x &&
		pt.y >= widget->box.bmin.y && pt.y < widget->box.bmax.y;
}

void Widget::draw() const
{
	WidgetDrawFunc draw_func = default_draw_func;

	if(theme) {
		draw_func = theme->get_draw_func(get_type_name());
	}

	draw_func(this);
}

// dummy event handlers
void Widget::on_mouse_button(const ButtonEvent &ev)
{
}

void Widget::on_mouse_motion(const MotionEvent &ev)
{
}

void Widget::on_mouse_focus(const FocusEvent &ev)
{
}

void Widget::on_key(const KeyEvent &ev)
{
}

void Widget::on_click()
{
}

void Widget::on_double_click()
{
}

void Widget::on_change()
{
}


/* the event dispatcher generates high-level events (click, etc)
 * and calls the on_whatever() functions for both low and high-level
 * events.
 * The on_whatever functions are called *after* any other actions performed
 * here, to give subclasses the opportunity to override them easily, by
 * overriding the on_ functions, without having to override handle_event itself
 */
// TODO also call callbacks here I guess...
void Widget::handle_event(const Event &ev)
{
	switch(ev.type) {
	case EV_MOUSE_BUTTON:
		if(ev.button.press) {
			press();
		} else {
			if(is_pressed()) {
				on_click();
			}
			release();
		}
		on_mouse_button(ev.button);
		break;

	case EV_MOUSE_MOTION:
		on_mouse_motion(ev.motion);
		break;

	case EV_MOUSE_FOCUS:
		if(ev.focus.enter) {
			mousein();
		} else {
			mouseout();
		}
		on_mouse_focus(ev.focus);
		break;

	case EV_KEY:
		on_key(ev.key);
		break;

	default:
		fprintf(stderr, "%s: unknown event id: %d\n", __func__, ev.type);
	}
}


}	// namespace goatkit
