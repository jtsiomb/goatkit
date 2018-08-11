/*
GoatKit - a themable/animated widget toolkit for games
Copyright (C) 2014-2018 John Tsiombikas <nuclear@member.fsf.org>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef GOATKIT_THEME_H_
#define GOATKIT_THEME_H_

#define GOATKIT_BUILTIN_THEME(n, f)	\
	static goatkit::Theme goatkit_theme##__LINE__(n, f)

namespace goatkit {

class Widget;
class Theme;
struct ThemeImpl;

typedef void (*WidgetDrawFunc)(const Widget*);
typedef WidgetDrawFunc (*WidgetLookupFunc)(const char*);

void add_theme_path(const char *path);
void default_draw_func(const Widget *w);

void register_theme(const char *name, Theme *theme);
Theme *get_theme(const char *name);

class Theme {
private:
	ThemeImpl *impl;

public:
	Theme();
	Theme(const char *name, WidgetLookupFunc func);
	~Theme();

	bool load(const char *name);
	void unload();

	WidgetDrawFunc get_draw_func(const char *type) const;
};

extern Theme *theme;	// the current theme

}	// namespace goatkit

#endif	// GOATKIT_THEME_H_
