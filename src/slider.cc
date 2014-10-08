#include <stdio.h>
#include "slider.h"

namespace goatkit {

struct SliderImpl {
	float value;
	bool dragging;
};

Slider::Slider()
{
	slider = new SliderImpl;
	slider->value = 0.0f;
	slider->dragging = false;
}

Slider::~Slider()
{
	delete slider;
}

const char *Slider::get_type_name() const
{
	return "slider";
}

void Slider::set_value(float val)
{
	slider->value = val;
}

float Slider::get_value() const
{
	return slider->value;
}

void Slider::on_mouse_button(const ButtonEvent &ev)
{
	if(ev.button == 0) {
		slider->dragging = ev.press;
	}
}

void Slider::on_mouse_motion(const MotionEvent &ev)
{
	if(!slider->dragging) {
		return;
	}

	BBox box = get_box();

	float new_val = (ev.pos.x - box.bmin.x) / (box.bmax.x - box.bmin.x);
	if(new_val != slider->value) {
		Event cev;
		cev.type = EV_CHANGE;
		handle_event(cev);
		slider->value = new_val;
	}
}

}	// namespace goatkit
