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

const char *Button::get_type_name() const
{
	return "button";
}

}	// namespace goatkit
