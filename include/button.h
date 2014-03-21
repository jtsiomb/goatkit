#ifndef GAMEUI_BUTTON_H_
#define GAMEUI_BUTTON_H_

#include "widget.h"

namespace gameui {

struct ButtonImpl;

class Button : public Widget {
private:
	ButtonImpl *button;

public:
	Button();
	virtual ~Button();
};

}

#endif	// GAMEUI_BUTTON_H_
