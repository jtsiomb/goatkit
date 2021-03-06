#include <string.h>
#include <string>
#include <map>
#include "goatkit.h"

#ifndef _MSC_VER
#include <alloca.h>
#else
#include <malloc.h>
#endif

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <drawtext.h>

#define FONT	GLUT_BITMAP_HELVETICA_18

using namespace goatkit;

typedef void (*DrawFunc)(const Widget*);

static void draw_button(const Widget *w);
static void draw_checkbox(const Widget *w);
static void draw_label(const Widget *w);
static void draw_slider(const Widget *w);
static void draw_textbox(const Widget *w);
static float calc_text_width(const char *text);
static void draw_text(float x, float y, const char *text);
static void draw_rect(const Widget *w, float x, float y, float xsz, float ysz);

static void get_fgcolor(const Widget *w, float *col);
static void get_bgcolor(const Widget *w, float *col);

static struct {
	const char *name;
	DrawFunc func;
} widget_funcs[] = {
	{ "button", draw_button },
	{ "checkbox", draw_checkbox },
	{ "label", draw_label },
	{ "slider", draw_slider },
	{ "textbox", draw_textbox },
	{ 0, 0 }
};

static bool initialized;
static std::map<std::string, DrawFunc> funcmap;

/* theme colors */
static float fgcol[] = {0.8, 0.6, 0.4, 1.0};
static float fgcol_off[] = {0.65, 0.65, 0.6, 1.0};
static float fgcol_inact[] = {0.4, 0.4, 0.4, 1.0};
static float bgcol[] = {0.3, 0.3, 0.3, 1.0};
static float bgcol_off[] = {0.3, 0.3, 0.3, 1.0};
static float bgcol_inact[] = {0.3, 0.3, 0.3, 1.0};


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
	float vis = w->get_visibility();

	if(vis < VIS_THRES) {
		return;
	}

	float tor_rad = sz.x * 0.5 * vis;
	float scale = LERP(1.0, 0.85, pressed);

	float fg[4], bg[4];
	get_fgcolor(w, fg);
	get_bgcolor(w, bg);

	begin_drawing(w);

	glTranslatef(sz.x / 2.0, sz.y / 2.0, 0.0);
	glScalef(1, 1, 1.0 / sz.x);
	glRotatef(90, 1, 0, 0);

	glColor4fv(fg);
	glutSolidTorus(scale * sz.y / 2.0, scale * tor_rad / 2.0, 18, 16);

	glScalef(1.0 - 0.1 / bnaspect, 1.0, 1.0 - 0.1);
	glColor4fv(bg);
	glutSolidTorus(scale * sz.y / 2.0, scale * tor_rad / 2.0, 18, 16);

	if(vis >= 1.0) {
		glTranslatef(-calc_text_width(w->get_text()) / 2.0, 0, -5);
		glColor4fv(fg);
		draw_text(0, 0, w->get_text());
	}

	end_drawing(w);
}

static void draw_checkbox(const Widget *w)
{
	Vec2 sz = w->get_size();
	float vis = w->get_visibility();

	if(vis < VIS_THRES) {
		return;
	}

	float fg[4];
	get_fgcolor(w, fg);

	goatkit::CheckBox *cbox = (goatkit::CheckBox*)w;

	begin_drawing(w);

	draw_rect(w, 0, 0, sz.y, sz.y);

	float state = cbox->get_checked();

	float bottom[2] = {sz.y / 2, sz.y - 1};
	float left[2] = {-2, sz.y / 4};
	float right[2] = {sz.y, -4};

	if(state > 0.0) {
		// draw tickmark
		glBegin(GL_TRIANGLES);
		glColor4fv(fg);
		float t = state * 2.0 > 1.0 ? 1.0 : state * 2.0;
		glVertex2f(left[0], left[1]);
		glVertex2f(LERP(left[0], bottom[0], t), LERP(left[1], bottom[1], t));
		glVertex2f(LERP(left[0], bottom[0], t), LERP(left[1], bottom[1] - sz.y / 3, t));

		if((t = state * 2.0 - 1.0) > 0.0) {
			glVertex2f(bottom[0], bottom[1]);
			glVertex2f(LERP(bottom[0], right[0], t), LERP(bottom[1], right[1], t));
			glVertex2f(bottom[0], bottom[1] - sz.y / 3);
		}
		glEnd();
	}

	glTranslatef(sz.y * 1.5, sz.y / 2.0 + 5, 0);
	glColor4fv(fg);
	draw_text(0, 0, w->get_text());

	end_drawing(w);
}

static void draw_label(const Widget *w)
{
	Vec2 sz = w->get_size();
	float vis = w->get_visibility();

	if(vis < VIS_THRES) {
		return;
	}

	float fg[4];
	get_fgcolor(w, fg);

	begin_drawing(w);

	glTranslatef((sz.x - calc_text_width(w->get_text())) / 2.0, sz.y / 2.0, 0);
	glColor4fv(fg);
	draw_text(0, 0, w->get_text());

	end_drawing(w);
}

static void draw_slider(const Widget *w)
{
	Vec2 sz = w->get_size();
	float vis = w->get_visibility();

	if(vis < VIS_THRES) {
		return;
	}

	float fg[4], bg[4];
	get_fgcolor(w, fg);
	get_bgcolor(w, bg);

	Slider *slider = (Slider*)w;
	float pad = slider->get_padding();
	float handle_width = pad * 2.0;

	float value = slider->get_value();
	char valtext[16];
	sprintf(valtext, "%g", value);

	float trough_sz = sz.x - 2.0 * pad;
	float x = pad + trough_sz * slider->get_value_norm();

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
			glVertex2f(x + 0.5, sz.y / 3.0 + 0.5);
			glVertex2f(x + 0.5, sz.y / 2.0 + 0.5);
			x += dx;
		}
		glEnd();
	}

	draw_rect(w, 0, sz.y - act_height + act_height / 3, sz.x, act_height / 3);
	draw_rect(w, x - handle_width / 2.0, sz.y - act_height, handle_width, act_height);
	draw_text(x - calc_text_width(valtext) / 2.0, act_height / 2.0, valtext);

	end_drawing(w);
}

static void draw_textbox(const Widget *w)
{
	if(!w->is_visible()) {
		return;
	}

	Vec2 sz = w->get_size();

	float fg[4];
	get_fgcolor(w, fg);

	TextBox *tbox = (TextBox*)w;

	begin_drawing(w);

	draw_rect(w, 0, 0, sz.x, sz.y);

	const char *str = tbox->get_text();
	char *buf = (char*)alloca(strlen(str) + 1);

	// figure out how many characters fit in the textbox
	float tsz = 0.0;
	const char *sptr = str;
	char *dptr = buf;
	while(*sptr) {
		float nsz = tsz + glutBitmapWidth(FONT, *sptr);
		if(nsz >= sz.x) {
			break;
		}
		*dptr++ = *sptr++;
		tsz = nsz;
	}
	*dptr = 0;

	glColor4fv(fg);
	draw_text(2, 2.0 * sz.y / 3.0, buf);

	// draw the cursor
	int cursor = tbox->get_cursor();
	float cx = 1.0;

	if(cursor > 0) {
		memcpy(buf, str, cursor);
		buf[cursor] = 0;
		cx += calc_text_width(buf);
	}

	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glColor4f(1.0 - fg[0], 1.0 - fg[1], 1.0 - fg[2], fg[3]);
	draw_text(cx, 2.0 * sz.y / 3.0, "|");
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	end_drawing(w);
}

static float calc_text_width(const char *text)
{
	float res = 0.0f;

	while(*text) {
		res += glutBitmapWidth(FONT, *text++);
	}
	return res;
}

static void draw_text(float x, float y, const char *text)
{
	glRasterPos2f(x, y);

	while(*text) {
		glutBitmapCharacter(FONT, *text++);
	}
}

#define DISP(s, t)		LERP((s) * 2.0, 0, (t))
static void draw_rect(const Widget *w, float x, float y, float xsz, float ysz)
{
	float fg[4], bg[4];
	float vis = w->get_visibility();

	get_fgcolor(w, fg);
	get_bgcolor(w, bg);

	glBegin(GL_QUADS);
	glColor4fv(bg);
	glVertex2f(x, y);
	glVertex2f(x + xsz, y);
	glVertex2f(x + xsz, y + ysz);
	glVertex2f(x, y + ysz);
	glEnd();

	glLineWidth(w->get_focus() + 1.0);

	glPushMatrix();
	glTranslatef(0.5, 0.5, 0);

	glBegin(GL_LINES);
	glColor4fv(fg);

	// top
	glVertex2f(x - DISP(xsz, vis), y);
	glVertex2f(x + xsz - DISP(xsz, vis), y);

	// right
	glVertex2f(x + xsz, y - DISP(ysz, vis));
	glVertex2f(x + xsz, y + ysz - DISP(ysz, vis));

	// bottom
	glVertex2f(x + xsz + DISP(xsz, vis), y + ysz);
	glVertex2f(x + DISP(xsz, vis), y + ysz);

	// left
	glVertex2f(x, y + ysz + DISP(ysz, vis));
	glVertex2f(x, y + DISP(ysz, vis));

	glEnd();
	glPopMatrix();

	glLineWidth(1);
}


static void get_fgcolor(const Widget *w, float *col)
{
	float hover = w->get_under_mouse();
	float act = w->get_active();
	float vis = w->get_visibility();

	for(int i=0; i<4; i++) {
		float c = LERP(fgcol_off[i], fgcol[i], hover);
		col[i] = LERP(fgcol_inact[i], c, act);
	}
	col[3] *= vis;
}

static void get_bgcolor(const Widget *w, float *col)
{
	float hover = w->get_under_mouse();
	float act = w->get_active();
	float vis = w->get_visibility();

	for(int i=0; i<4; i++) {
		float c = LERP(bgcol_off[i], bgcol[i], hover);
		col[i] = LERP(bgcol_inact[i], c, act);
	}
	col[3] *= vis;
}
