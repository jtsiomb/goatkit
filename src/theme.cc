#include <stdio.h>
#include "theme.h"
#include "widget.h"

#ifdef WIN32
#include <windows.h>
#endif
#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif


namespace gameui {

Theme *theme;

Theme::Theme()
{
	so = 0;
}

bool Theme::load(const char *name)
{
	fprintf(stderr, "theme loading not implemented yet!\n");
	return false;
}

widget_draw_func Theme::get_draw_func(const char *type) const
{
	std::map<std::string, widget_draw_func>::const_iterator it = draw_func.find(type);
	if(it == draw_func.end()) {
		return default_draw_func;
	}
	return it->second;
}

#define LERP(a, b, t)	((a) + ((b) - (a)) * t)
#define DEF_TEX_SZ	32
void default_draw_func(const Widget *w)
{
	static unsigned int tex;

	if(!tex) {
		unsigned char *pixels = new unsigned char[DEF_TEX_SZ * DEF_TEX_SZ * 3];
		unsigned char *ptr = pixels;
		for(int i=0; i<DEF_TEX_SZ; i++) {
			for(int j=0; j<DEF_TEX_SZ; j++) {
				bool stripe = (((j + i) / 8) & 1) == 1;
				ptr[0] = ptr[1] = ptr[2] = stripe ? 255 : 0;
				ptr += 3;
			}
		}

		glGenTextures(1, &tex);
		glBindTexture(GL_TEXTURE_2D, tex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, DEF_TEX_SZ, DEF_TEX_SZ, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);
		delete [] pixels;
	}

	Vec2 pos = w->get_position();
	Vec2 sz = w->get_size();
	float aspect = sz.x / sz.y;

	glPushAttrib(GL_ENABLE_BIT);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, tex);

	float offs = w->get_pressed() * 0.1 * sz.y;
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glTranslatef(offs, -offs, 0);

	float active = w->get_active();
	float hover = w->get_under_mouse();

	float rg = LERP(0.4, 1.0, hover);
	float b = LERP(rg, 0, active);
	glColor3f(rg, rg, b);

	glBegin(GL_QUADS);
	glTexCoord2f(0, 1);
	glVertex2f(pos.x, pos.y);
	glTexCoord2f(aspect, 1);
	glVertex2f(pos.x + sz.x, pos.y);
	glTexCoord2f(aspect, 0);
	glVertex2f(pos.x + sz.x, pos.y + sz.y);
	glTexCoord2f(0, 0);
	glVertex2f(pos.x, pos.y + sz.y);
	glEnd();

	glPopMatrix();

	glPopAttrib();
}

}	// namespace gameui
