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
static void draw_label(const Widget *w);
static float calc_text_width(const char *text);
static void draw_text(float x, float y, const char *text);

static struct {
	const char *name;
	DrawFunc func;
} widget_funcs[] = {
	{ "button", draw_button },
	{ "label", draw_label },
	{ 0, 0 }
};

static bool initialized;
static std::map<std::string, DrawFunc> funcmap;

/* theme colors */
static float fgcol[] = {0.6, 0.6, 0.6, 1.0};
static float fgcol_off[] = {0.8, 0.6, 0.4, 1.0};
static float bgcol[] = {0.3, 0.3, 0.3, 1.0};

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

#define VIS_THRES	0.0001

static void begin_drawing(const Widget *w)
{
	Vec2 pos = w->get_position();

	glPushAttrib(GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT);
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glTranslatef(pos.x, pos.y, 0);
}

static void end_drawing(const Widget *w)
{
	glPopMatrix();
	glPopAttrib();
}


#define LERP(a, b, t)	((a) + ((b) - (a)) * t)

static void draw_button(const Widget *w)
{
	Vec2 sz = w->get_size();
	float bnaspect = sz.x / sz.y;

	float pressed = w->get_pressed();
	//float active = w->get_active();
	float hover = w->get_under_mouse();
	float vis = w->get_visibility();

	if(vis < VIS_THRES) {
		return;
	}

	float tor_rad = sz.x * 0.5 * vis;
	float scale = LERP(1.0, 0.85, pressed);

	float bcol[] = {
		LERP(fgcol_off[0], fgcol[0], hover),
		LERP(fgcol_off[1], fgcol[1], hover),
		LERP(fgcol_off[2], fgcol[2], hover)
	};

	begin_drawing(w);

	glTranslatef(sz.x / 2.0, sz.y / 2.0, 0.0);
	glScalef(1, 1, 1.0 / sz.x);
	glRotatef(90, 1, 0, 0);

	glColor3f(bcol[0], bcol[1], bcol[2]);
	glutSolidTorus(scale * sz.y / 2.0, scale * tor_rad / 2.0, 18, 16);

	glScalef(1.0 - 0.1 / bnaspect, 1.0, 1.0 - 0.1);
	glColor3fv(bgcol);
	glutSolidTorus(scale * sz.y / 2.0, scale * tor_rad / 2.0, 18, 16);

	if(vis >= 1.0) {
		glTranslatef(-calc_text_width(w->get_text()) / 2.0, 0, -5);
		glColor3f(bcol[0], bcol[1], bcol[2]);
		draw_text(0, 0, w->get_text());
	}

	end_drawing(w);
}

static void draw_label(const Widget *w)
{
	Vec2 sz = w->get_size();
	float vis = w->get_visibility();

	if(vis < VIS_THRES) {
		return;
	}

	begin_drawing(w);

	glTranslatef((sz.x - calc_text_width(w->get_text())) / 2.0, sz.y / 2.0, 0);
	glColor4f(fgcol[0], fgcol[1], fgcol[2], vis);
	draw_text(0, 0, w->get_text());

	end_drawing(w);
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
