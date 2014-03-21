#ifndef THEME_H_
#define THEME_H_

#include <string>
#include <map>
#include "goatkit.h"

namespace goatkit {

class Widget;

void add_theme_path(const char *path);
void default_draw_func(const Widget *w);

class Theme {
private:
	void *so;
	WidgetDrawFunc (*lookup_theme_draw_func)(const char*);
	mutable std::map<std::string, WidgetDrawFunc> func_cache;

public:
	Theme();
	~Theme();

	bool load(const char *name);
	void unload();

	WidgetDrawFunc get_draw_func(const char *type) const;
};

extern Theme *theme;	// the current theme

}	// namespace goatkit

#endif	// THEME_H_
