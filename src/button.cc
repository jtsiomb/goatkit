#include "button.h"

namespace goatkit {

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

}	// namespace goatkit
