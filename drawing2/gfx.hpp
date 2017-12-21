/*
* File:   gfx.hpp
* Author: darek
*
* Created on 25 November 2014, 11:20
*/
#ifndef QFRACTAL_GFX_HPP
#define QFRACTAL_GFX_HPP

#include "utils.hpp"
#include <GL/glut.h>

#define  GFX_MOUSE_LEFT					GLUT_LEFT_BUTTON
#define  GFX_MOUSE_MIDDLE				GLUT_MIDDLE_BUTTON
#define  GFX_MOUSE_RIGHT				GLUT_RIGHT_BUTTON
#define  GFX_MOUSE_WHEELUP				3
#define  GFX_MOUSE_WHEELDOWN			4
#define  GFX_MOUSE_PRESS				GLUT_DOWN
#define  GFX_MOUSE_RELEASE				GLUT_UP
#define  GFX_MOUSE_CLICK				((MOUSE_PRESS+MOUSE_RELEASE)<<1)
#define  GFX_MOUSE_ENTER				GLUT_ENTERED
#define  GFX_MOUSE_EXIT					GLUT_LEFT

class gfx {
public:
	static void warpPointer(int x, int y) { glutWarpPointer(x, y); }
	static int  windowWidth() { return glutGet(GLUT_WINDOW_WIDTH); }
	static int  windowHeight() { return glutGet(GLUT_WINDOW_HEIGHT); }
	static void setRefresh() { glutPostRedisplay(); }
	static void swapBuffers() { glFlush(); glutSwapBuffers(); }
	static void setWindowTitle(const char *title) { glutSetWindowTitle(title); }
private:
	gfx() { }
	~gfx() { }
	gfx& operator=(const gfx &o) { return *this; }
};

#endif
