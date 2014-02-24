#ifndef GAMEUI_BUTTON_H_
#define GAMEUI_BUTTON_H_

#include "widget.h"

namespace gameui {

class ButtonImpl;

class Button {
private:
	ButtonImpl *impl;

public:
	Button();
	virtual ~Button();
};

}

#endif	// GAMEUI_BUTTON_H_
