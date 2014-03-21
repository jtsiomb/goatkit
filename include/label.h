#ifndef GOATKIT_LABEL_H_
#define GOATKIT_LABEL_H_

namespace goatkit {

class LabelImpl;

class Label {
private:
	LabelImpl *impl;

public:
	Label();
	virtual ~Label();
};

}	// namespace goatkit

#endif	// GOATKIT_LABEL_H_
