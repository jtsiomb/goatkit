#include <stdio.h>
#include "slider.h"

namespace goatkit {

struct SliderImpl {
	float value, prev_value;
	bool dragging;
	bool cont_change;
};

Slider::Slider()
{
	slider = new SliderImpl;
	slider->value = slider->prev_value = 0.0f;
	slider->dragging = false;
	slider->cont_change = true;
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

void Slider::set_continuous_change(bool cont)
{
	slider->cont_change = cont;
}

bool Slider::get_continuous_change() const
{
	return slider->cont_change;
}

void Slider::on_mouse_button(const ButtonEvent &ev)
{
	if(ev.button == 0) {
		slider->dragging = ev.press;
		if(!ev.press) {
			// on release, if the value has changed send the appropriate event
			if(slider->prev_value != slider->value) {
				Event ev;
				ev.type = EV_CHANGE;
				handle_event(ev);
				slider->prev_value = slider->value;
			}
		}
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
		slider->value = new_val;
		if(slider->cont_change) {
			Event cev;
			cev.type = EV_CHANGE;
			handle_event(cev);
		}
	}
}

}	// namespace goatkit
