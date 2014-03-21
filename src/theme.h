#ifndef THEME_H_
#define THEME_H_

#include <string>
#include <map>

namespace gameui {

class Widget;

typedef void (*widget_draw_func)(const Widget*);

void default_draw_func(const Widget *w);

class Theme {
private:
	void *so;
	std::map<std::string, widget_draw_func> draw_func;

public:
	Theme();
	~Theme();

	bool load(const char *name);

	widget_draw_func get_draw_func(const char *type) const;
};

extern Theme *theme;	// the current theme

}	// namespace gameui

#endif	// THEME_H_
