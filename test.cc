#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <vector>
#include <GL/glut.h>
#include "gameui.h"

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

static std::vector<gameui::Widget*> widgets;

int main(int argc, char **argv)
{
	glutInitWindowSize(800, 600);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
	glutCreateWindow("gameui test");

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
	gameui::Button *button = new gameui::Button;
	button->set_position(350, 280);
	button->set_size(100, 40);
	widgets.push_back(button);

	return true;
}

static void cleanup()
{
	for(size_t i=0; i<widgets.size(); i++) {
		delete widgets[i];
	}
}

static void disp()
{
	glClearColor(0.2, 0.2, 0.2, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for(size_t i=0; i<widgets.size(); i++) {
		widgets[i]->draw();
	}

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
	glOrtho(0, x, 0, y, -1, 1);
}

static void keypress(unsigned char key, int x, int y)
{
	if(key == 27) {
		exit(0);
	}
}

static void keyrelease(unsigned char key, int x, int y)
{
}

static void skeypress(int key, int x, int y)
{
}

static void skeyrelease(int key, int x, int y)
{
}

static void mouse(int bn, int st, int x, int y)
{
	int bidx = bn - GLUT_LEFT_BUTTON;
	bool down = st == GLUT_DOWN;

	for(size_t i=0; i<widgets.size(); i++) {
		gameui::Widget *w = widgets[i];

		if(w->hit_test(gameui::Vec2(x, y))) {
			gameui::Event ev;
			ev.type = gameui::EV_MOUSE_BUTTON;
			ev.button.button = bidx;
			ev.button.press = down;
			ev.button.pos = gameui::Vec2(x, y);
			w->handle_event(ev);
		}
	}
}

static void motion(int x, int y)
{
	static gameui::Widget *active;

	if(active && !active->hit_test(gameui::Vec2(x, y))) {
		gameui::Event ev;
		ev.type = gameui::EV_MOUSE_FOCUS;
		ev.focus.enter = false;
		active->handle_event(ev);
		active = 0;
	}

	for(size_t i=0; i<widgets.size(); i++) {
		gameui::Widget *w = widgets[i];

		if(w->hit_test(gameui::Vec2(x, y))) {
			if(active != w) {
				gameui::Event ev;
				ev.type = gameui::EV_MOUSE_FOCUS;
				ev.focus.enter = true;
				w->handle_event(ev);
				active = w;
			}
		}
	}
}
