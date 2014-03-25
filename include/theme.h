#ifndef THEME_H_
#define THEME_H_

namespace goatkit {

class Widget;

typedef void (*WidgetDrawFunc)(const Widget*);

void add_theme_path(const char *path);
void default_draw_func(const Widget *w);

struct ThemeImpl;

class Theme {
private:
	ThemeImpl *impl;

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
