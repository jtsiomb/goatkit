/*
GoatKit - a themable/animated widget toolkit for games
Copyright (C) 2014-2018  John Tsiombikas <nuclear@member.fsf.org>

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
#ifndef GOATKIT_CHECKBOX_H_
#define GOATKIT_CHECKBOX_H_

#include "widget.h"

namespace goatkit {

struct CheckBoxImpl;

class CheckBox : public Widget {
private:
	CheckBoxImpl *cbox;

public:
	CheckBox();
	virtual ~CheckBox();

	virtual const char *get_type_name() const;

	virtual void check();
	virtual void uncheck();
	virtual float get_checked() const;
	virtual bool is_checked() const;
	virtual void toggle();
	virtual void set_toggle_transition(long msec);
	virtual long get_toggle_transition() const;

	virtual void on_click();
};

}	// namespace goatkit

#endif	// GOATKIT_CHECKBOX_H_
