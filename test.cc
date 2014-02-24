#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <GL/glut.h>

static bool init();
static void cleanup();
static void disp();
static void reshape(int x, int y);
static void keypress(unsigned char key, int x, int y);
static void keyrelease(unsigned char key, int x, int y);
static void skeypress(int key, int x, int y);
static void skeyrelease(int key, int x, int y);
static void mouse(int bn, int st, int x, int y);
static void motion(int x, int y);

int main(int argc, char **argv)
{
	glutInitWindowSize(800, 600);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
	glutCreateWindow("gameui test");

	glutDisplayFunc(disp);
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
	return true;
}

static void cleanup()
{
}

static void disp()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glutSwapBuffers();
	assert(glGetError() == GL_NO_ERROR);
}

static void reshape(int x, int y)
{
	glViewport(0, 0, x, y);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, x, y, 0, -1, 1);
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
}

static void motion(int x, int y)
{
}
