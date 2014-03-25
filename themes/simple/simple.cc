#include <string>
#include <map>
#include "goatkit.h"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

using namespace goatkit;

typedef void (*DrawFunc)(const Widget*);

static void draw_button(const Widget *w);

static struct {
	const char *name;
	DrawFunc func;
} widget_funcs[] = {
	{ "button", draw_button },
	{ 0, 0 }
};

static std::map<std::string, DrawFunc> funcmap;

extern "C" goatkit::WidgetDrawFunc get_widget_func(const char *name)
{
	int i;

	if(funcmap.empty()) {
		for(i=0; widget_funcs[i].func; i++) {
			funcmap[widget_funcs[i].name] = widget_funcs[i].func;
		}
	}
	return funcmap[name];
}

static void draw_button(const Widget *w)
{
	Vec2 pos = w->get_position();
	Vec2 sz = w->get_size();

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glTranslatef(pos.x + sz.x / 2.0, pos.y + sz.y / 2.0, 0.0);
	glScalef(1, 1, 1.0 / sz.x);
	glRotatef(90, 1, 0, 0);
	glutSolidTorus(sz.y, sz.x * 0.5, 18, 16);

	glPopMatrix();
}
