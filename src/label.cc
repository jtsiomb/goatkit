#include "label.h"

namespace goatkit {

struct LabelImpl {
};

Label::Label()
{
	label = new LabelImpl;
}

Label::~Label()
{
	delete label;
}

const char *Label::get_type_name() const
{
	return "label";
}

}	// namespace goatkit
