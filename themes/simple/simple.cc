#include <string>
#include <map>
#include "goatkit.h"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <drawtext.h>

using namespace goatkit;

typedef void (*DrawFunc)(const Widget*);

static void draw_button(const Widget *w);
static float calc_text_width(const char *text);
static void draw_text(float x, float y, const char *text);

static struct {
	const char *name;
	DrawFunc func;
} widget_funcs[] = {
	{ "button", draw_button },
	{ 0, 0 }
};

static bool initialized;
static std::map<std::string, DrawFunc> funcmap;

extern "C" goatkit::WidgetDrawFunc get_widget_func(const char *name)
{
	int i;

	if(!initialized) {
		for(i=0; widget_funcs[i].func; i++) {
			funcmap[widget_funcs[i].name] = widget_funcs[i].func;
		}

		initialized = true;
	}
	return funcmap[name];
}


#define LERP(a, b, t)	((a) + ((b) - (a)) * t)

static void draw_button(const Widget *w)
{
	Vec2 pos = w->get_position();
	Vec2 sz = w->get_size();
	float bnaspect = sz.x / sz.y;

	float pressed = w->get_pressed();
	float active = w->get_active();
	float hover = w->get_under_mouse();
	float vis = w->get_visibility();

	if(vis < 0.0001) {
		return;
	}

	float tor_rad = sz.x * 0.5 * vis;
	float bcol_hoff[] = {0.6, 0.6, 0.6};
	float bcol_hon[] = {0.8, 0.6, 0.4};
	float bcol[] = {
		LERP(bcol_hoff[0], bcol_hon[0], hover),
		LERP(bcol_hoff[1], bcol_hon[1], hover),
		LERP(bcol_hoff[2], bcol_hon[2], hover)
	};

	glPushAttrib(GL_ENABLE_BIT);
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glTranslatef(pos.x + sz.x / 2.0, pos.y + sz.y / 2.0, 0.0);
	glScalef(1, 1, 1.0 / sz.x);
	glRotatef(90, 1, 0, 0);

	glColor3f(bcol[0], bcol[1], bcol[2]);
	glutSolidTorus(sz.y / 2.0, tor_rad / 2.0, 18, 16);

	glScalef(1.0 - 0.1 / bnaspect, 1.0, 1.0 - 0.1);
	glColor3f(0.3, 0.3, 0.3);
	glutSolidTorus(sz.y / 2.0, tor_rad / 2.0, 18, 16);

	if(vis >= 1.0) {
		glTranslatef(-calc_text_width(w->get_text()) / 2.0, 0, -5);
		glColor3f(bcol[0], bcol[1], bcol[2]);
		draw_text(0, 0, w->get_text());
	}
	glPopMatrix();

	//glRectf(pos.x, pos.y, pos.x + sz.x, pos.y + sz.y);

	glPopAttrib();
}

static float calc_text_width(const char *text)
{
	float res = 0.0f;

	while(*text) {
		res += glutBitmapWidth(GLUT_BITMAP_HELVETICA_18, *text++);
	}
	return res;
}

static void draw_text(float x, float y, const char *text)
{
	glRasterPos2f(x, y);

	while(*text) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *text++);
	}
}
