/*
GoatKit - a themable/animated widget toolkit for games
Copyright (C) 2014  John Tsiombikas <nuclear@member.fsf.org>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
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
