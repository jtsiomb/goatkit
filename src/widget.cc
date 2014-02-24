#include <string>
#include <stringstream>
#include "widget.h"

using namespace gameui;

class WidgetImpl {
public:
	std::string name_prefix;
	std::string name;
	BBox box;
	VisState vis_st;
	ActiveState act_st;

	float vis, act;

	long vis_start_time, act_start_time;
};


Widget::Widget()
	: name_prefix("widget")
{
	static int widget_count;

	std::stringstream sstr;
	sstr << name_prefix << widget_count++;
	name = sstr.str();

	box.bmin = Vec2(0, 0);
	box.bmax = Vec2(1, 1);

	vis_st = VST_VISIBLE;
	act_st = AST_ACTIVE;

	vis = act = 1.0;
}

Widget::~Widget()
{
}

void Widget::show()
{
	if(vis_st == VST_EASEVISIBLE || vis_st == VST_EASEIN) {
		return;
	}

	vis_st = VST_EASEIN;
	vis_start_time = get_cur_time();
}

void Widget::hide()
{
	if(vis_st == VST_EASEHIDDEN || vis_st == VST_EASEOUT) {
		return;
	}

	vis_st = VST_EASEOUT;
	vis_start_time = get_cur_time();
}

float Widget::get_visibility() const
{
	switch(vis_st) {
	case VST_EASEIN:
		vis = (get_cur_time() - vis_start_time) / gameui::ease_time;
		if(vis < 0.0) vis = 0.0;
		if(vis > 1.0) vis = 1.0;
		break;

	case VST_EASEOUT:
		vis = 1.0 - (get_cur_time() - vis_start_time) / gameui::ease_time;
		if(vis < 0.0) vis = 0.0;
		if(vis > 1.0) vis = 1.0;
		break;

	case VST_HIDDEN:
		vis = 0.0;
		break;

	case VST_VISIBLE:
		vis = 1.0;
		break;
	}

	return vis;
}

#ifdef WIN32
long gameui::get_cur_time()
{
	return GetTickCount();
}
#endif

#if defined(__unix__) || defined(__APPLE__)
long gameui::get_cur_time()
{
	struct timeval tv;
	static struct timeval tv0;

	gettimeofday(&tv, 0);
	if(tv0.tv_sec == 0 && tv0.tv_msec == 0) {
		tv0 = tv;
		return 0;
	}
	return (tv.tv_sec - tv0.tv_sec) * 1000 + (tv.tv_usec - tv0.tv_usec) / 1000;
}
#endif
