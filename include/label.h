#ifndef GOATKIT_LABEL_H_
#define GOATKIT_LABEL_H_

#include "widget.h"

namespace goatkit {

struct LabelImpl;

class Label : public Widget {
private:
	LabelImpl *label;

public:
	Label();
	virtual ~Label();

	virtual const char *get_type_name() const;
};

}	// namespace goatkit

#endif	// GOATKIT_LABEL_H_
