#include <stdio.h>
#include <vector>
#include <algorithm>
#include "theme.h"
#include "widget.h"

#ifdef WIN32
#include <windows.h>

static void *dlopen(const char *name, int flags);
static void dlclose(void *so);
static void dlsym(void *so, const char *symbol);
#else
#include <unistd.h>
#include <dlfcn.h>
#endif

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif


namespace goatkit {

Theme *theme;
static std::vector<std::string> search_paths;


void add_theme_path(const char *path)
{
	if(!path || !*path) return;

	std::string s = path;
	int last = s.length() - 1;
	if(s[last] == '/' || s[last] == '\\') {
		s.erase(last);
	}

	if(std::find(search_paths.begin(), search_paths.end(), s) != search_paths.end()) {
		return;
	}

	search_paths.push_back(s);
}

Theme::Theme()
{
	so = 0;
}

Theme::~Theme()
{
	unload();
}

typedef WidgetDrawFunc (*LookupFunc)(const char*);

bool Theme::load(const char *name)
{
	unload();

	if(!(so = dlopen(name, RTLD_LAZY))) {
		for(size_t i=0; i<search_paths.size(); i++) {
			std::string path = search_paths[i] + std::string(name);

			if((so = dlopen(path.c_str(), RTLD_LAZY))) {
				break;
			}
		}

		if(!so) {
			fprintf(stderr, "%s: failed to load theme plugin: %s\n", __func__, name);
			return false;
		}
	}

	// loaded the shared object, now get the lookup function
	lookup_theme_draw_func = (LookupFunc)dlsym(so, "get_widget_count");
	if(!lookup_theme_draw_func) {
		fprintf(stderr, "%s: invalid theme plugin %s\n", __func__, name);
		unload();
		return false;
	}

	return true;
}

void Theme::unload()
{
	if(so) {
		dlclose(so);
		so = 0;
	}
	func_cache.clear();
}

WidgetDrawFunc Theme::get_draw_func(const char *type) const
{
	std::map<std::string, WidgetDrawFunc>::const_iterator it = func_cache.find(type);
	if(it == func_cache.end()) {
		// don't have it cached, try to look it up
		WidgetDrawFunc func;
		if(lookup_theme_draw_func && (func = lookup_theme_draw_func(type))) {
			func_cache[type] = func;
			return func;
		}

		// can't look it up, return the default
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

}	// namespace goatkit

#ifdef WIN32
// XXX untested
static void *dlopen(const char *name, int flags)
{
	return LoadLibrary(name);
}

static void dlclose(void *so)
{
	// TODO
}

static void dlsym(void *so, const char *symbol)
{
	return GetProcAddress(so, symbol);
}
#endif
