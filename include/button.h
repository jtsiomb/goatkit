#ifndef GOATKIT_BUTTON_H_
#define GOATKIT_BUTTON_H_

#include "widget.h"

namespace goatkit {

struct ButtonImpl;

class Button : public Widget {
private:
	ButtonImpl *button;

public:
	Button();
	virtual ~Button();
};

}	// namespace goatkit

#endif	// GOATKIT_BUTTON_H_
