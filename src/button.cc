#include "button.h"

namespace gameui {

struct ButtonImpl {
};

Button::Button()
{
	button = new ButtonImpl;
}

Button::~Button()
{
	delete button;
}

}	// namespace gameui
