#ifndef EVENT_H_
#define EVENT_H_

#include "vec.h"

namespace gameui {

enum EventType {
	EV_MOUSE_BUTTON,
	EV_MOUSE_MOTION,
	EV_MOUSE_FOCUS,
	EV_KEY
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

}	// namespace gameui

#endif	// EVENT_H_
