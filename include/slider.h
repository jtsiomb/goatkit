#ifndef SLIDER_H_
#define SLIDER_H_

#include "widget.h"

namespace goatkit {

struct SliderImpl;

class Slider : public Widget {
private:
	SliderImpl *slider;

public:
	Slider();
	virtual ~Slider();

	virtual const char *get_type_name() const;

	virtual void set_value(float val);
	virtual float get_value() const;

	virtual void set_continuous_change(bool cont);
	virtual bool get_continuous_change() const;

	virtual void on_mouse_button(const ButtonEvent &ev);
	virtual void on_mouse_motion(const MotionEvent &ev);
};

}	// namespace goatkit

#endif	// SLIDER_H_
