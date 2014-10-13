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
static void draw_checkbox(const Widget *w);
static void draw_label(const Widget *w);
static void draw_slider(const Widget *w);
static float calc_text_width(const char *text);
static void draw_text(float x, float y, const char *text);
static void draw_rect(float x, float y, float xsz, float ysz, float fg[4], float bg[4]);

static struct {
	const char *name;
	DrawFunc func;
} widget_funcs[] = {
	{ "button", draw_button },
	{ "checkbox", draw_checkbox },
	{ "label", draw_label },
	{ "slider", draw_slider },
	{ 0, 0 }
};

static bool initialized;
static std::map<std::string, DrawFunc> funcmap;

/* theme colors */
static float fgcol[] = {0.8, 0.6, 0.4, 1.0};
static float fgcol_off[] = {0.6, 0.6, 0.6, 1.0};
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

static void draw_checkbox(const Widget *w)
{
	begin_drawing(w);
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
	glColor4f(fgcol_off[0], fgcol_off[1], fgcol_off[2], vis);
	draw_text(0, 0, w->get_text());

	end_drawing(w);
}

static void draw_slider(const Widget *w)
{
	Vec2 sz = w->get_size();
	float vis = w->get_visibility();
	float hover = w->get_under_mouse();

	if(vis < VIS_THRES) {
		return;
	}


	Slider *slider = (Slider*)w;
	float pad = slider->get_padding();
	float handle_width = pad * 2.0;

	float value = slider->get_value();
	char valtext[16];
	sprintf(valtext, "%g", value);

	float trough_sz = sz.x - 2.0 * pad;
	float x = pad + trough_sz * slider->get_value_norm();

	float fg[4] = {0, 0, 0, vis};
	for(int i=0; i<3; i++) {
		fg[i] = LERP(fgcol_off[i], fgcol[i], hover);
	}

	float act_height = sz.y / 2.0;

	begin_drawing(w);

	float step = slider->get_step();
	if(step > 0.0) {
		float beg = slider->get_range_min();
		float end = slider->get_range_max();
		int num_seg = (end - beg) / step;
		int num_ticks = num_seg + 1;
		float x = pad;
		float dx = trough_sz / num_seg;

		glLineWidth(1.0);
		glBegin(GL_LINES);
		glColor4fv(fg);
		for(int i=0; i<num_ticks; i++) {
			glVertex2f(x, sz.y / 3.0);
			glVertex2f(x, sz.y / 2.0);
			x += dx;
		}
		glEnd();
	}

	draw_rect(0, sz.y - act_height + act_height / 3, sz.x, act_height / 3, fg, bgcol);
	draw_rect(x - handle_width / 2.0, sz.y - act_height, handle_width, act_height, fg, bgcol);
	draw_text(x - calc_text_width(valtext) / 2.0, act_height / 2.0, valtext);

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

static void draw_rect(float x, float y, float xsz, float ysz, float fg[4], float bg[4])
{
	glBegin(GL_QUADS);
	glColor4fv(bg);
	glVertex2f(x, y);
	glVertex2f(x + xsz, y);
	glVertex2f(x + xsz, y + ysz);
	glVertex2f(x, y + ysz);
	glEnd();

	glBegin(GL_LINE_LOOP);
	glColor4fv(fg);
	glVertex2f(x, y);
	glVertex2f(x + xsz, y);
	glVertex2f(x + xsz, y + ysz);
	glVertex2f(x, y + ysz);
	glEnd();
}
