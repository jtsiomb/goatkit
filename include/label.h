#ifndef GAMEUI_LABEL_H_
#define GAMEUI_LABEL_H_

namespace gameui {

class LabelImpl;

class Label {
private:
	LabelImpl *impl;

public:
	Label();
	virtual ~Label();
};

}

#endif	// GAMEUI_LABEL_H_
