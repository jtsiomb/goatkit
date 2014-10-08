#ifndef EVENT_H_
#define EVENT_H_

#include "vec.h"

namespace goatkit {

enum EventType {
	// primary events
	EV_MOUSE_BUTTON,
	EV_MOUSE_MOTION,
	EV_MOUSE_FOCUS,
	EV_KEY,

	// derived events
	EV_CLICK,
	EV_DOUBLE_CLICK,
	EV_CHANGE,

	NUM_EVENTS
};

struct ButtonEvent {
	Vec2 pos;
	int button;
	bool press;
};

struct MotionEvent {
	Vec2 pos;
};

struct FocusEvent {
	bool enter;
};

struct KeyEvent {
	int key;
	bool press;
};

struct Event {
	EventType type;

	ButtonEvent button;
	MotionEvent motion;
	FocusEvent focus;
	KeyEvent key;
};

const char *event_type_name(EventType type);

}	// namespace goatkit

#endif	// EVENT_H_
