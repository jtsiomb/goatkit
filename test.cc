#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <vector>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include "goatkit.h"

static bool init();
static void cleanup();
static void disp();
static void idle();
static void reshape(int x, int y);
static void keypress(unsigned char key, int x, int y);
static void keyrelease(unsigned char key, int x, int y);
static void skeypress(int key, int x, int y);
static void skeyrelease(int key, int x, int y);
static void mouse(int bn, int st, int x, int y);
static void motion(int x, int y);

static goatkit::Screen scr;

int main(int argc, char **argv)
{
	glutInitWindowSize(800, 600);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
	glutCreateWindow("goatkit test");

	glutDisplayFunc(disp);
	glutIdleFunc(idle);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keypress);
	glutKeyboardUpFunc(keyrelease);
	glutSpecialFunc(skeypress);
	glutSpecialUpFunc(skeyrelease);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutPassiveMotionFunc(motion);

	if(!init()) {
		return 1;
	}
	atexit(cleanup);

	glutMainLoop();
	return 0;
}


static bool init()
{
	goatkit::Button *button = new goatkit::Button;
	button->set_position(300, 270);
	button->set_size(200, 60);
	button->set_text("a button!");
	button->hide();
	button->show();
	scr.add_widget(button);

	// load the theme
	goatkit::add_theme_path("themes/simple");

	goatkit::theme = new goatkit::Theme;
	goatkit::theme->load("simple");

	return true;
}

static void cleanup()
{
}

static void disp()
{
	glClearColor(0.2, 0.2, 0.2, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	scr.draw();

	glutSwapBuffers();
	assert(glGetError() == GL_NO_ERROR);
}

static void idle()
{
	glutPostRedisplay();
}

static void reshape(int x, int y)
{
	glViewport(0, 0, x, y);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, x, y, 0, -1, 1);

	scr.set_size(x, y);
}

static void keypress(unsigned char key, int x, int y)
{
	switch(key) {
	case 27:
		exit(0);
	}

	scr.sysev_keyboard(key, true);
}

static void keyrelease(unsigned char key, int x, int y)
{
	scr.sysev_keyboard(key, false);
}

static void skeypress(int key, int x, int y)
{
	scr.sysev_keyboard(key, true);
}

static void skeyrelease(int key, int x, int y)
{
	scr.sysev_keyboard(key, false);
}

static void mouse(int bn, int st, int x, int y)
{
	int bidx = bn - GLUT_LEFT_BUTTON;
	bool down = st == GLUT_DOWN;

	scr.sysev_mouse_button(bidx, down, x, y);
}

static void motion(int x, int y)
{
	scr.sysev_mouse_motion(x, y);
}
