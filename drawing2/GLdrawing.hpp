/*
* File:   GLdrawing.hpp
* Author: darek
*
* Created on 10 December 2014, 14:31
*/
#ifndef QFRACTAL_GLDRAWING_HPP
#define QFRACTAL_GLDRAWING_HPP

#include "utils.hpp"
#include "gfx.hpp"
#include "Color.hpp"

struct GLdrawingItem {
	GLfloat			*vertices;
	GLuint			*indices;
	GLubyte			*colors;
	GLenum			mode;
	GLsizei			count;
	ValueColor4f	color;
	GLint			color_channels;

	GLdrawingItem() 
		: vertices(NULL), indices(NULL), colors(NULL), mode(0), count(0), color_channels(3){}

	void draw() const 
	{
		if (isCreated()) {
			if (! colors) // .colors == NULL -> only one .color 
				glColor4fv(color.v);

			// activate and specify pointer to vertex array
			glEnableClientState(GL_VERTEX_ARRAY);
			glVertexPointer(3, GL_FLOAT, 0, vertices);

			if (colors) { // .colors != NULL -> has array of colors
				glEnableClientState(GL_COLOR_ARRAY);
				glColorPointer(color_channels, GL_UNSIGNED_BYTE, 0, colors);
			}

			// draw object
			if (indices) // has indices
				glDrawElements(mode, count, GL_UNSIGNED_INT, indices);
			else // no indices 
				glDrawArrays(mode, 0, count);

			// deactivate vertex arrays after drawing
			glDisableClientState(GL_VERTEX_ARRAY);

			if (colors) // deactivate colors array if exists
				glDisableClientState(GL_COLOR_ARRAY);
		}
	}

	bool isCreated() const { return vertices != NULL; }
};

class GLdrawing {
	GLdrawingItem	*m_items;
	GLuint			m_item_count;
	GLuint			m_curr_draw_item;
public:
	GLdrawing() 
		: m_items(NULL), m_item_count(0), m_curr_draw_item(0) { }

	~GLdrawing() {
		//fprintf(stderr, "[DBG] %s @ %d [DEL] %p (GLdrawingItem[] m_items)\n", __FILE__, __LINE__, m_items);
		delete[] m_items; m_items = NULL;
	}

	GLdrawingItem* getItem(GLuint index);

	int init(GLuint count_display_objects);
	
	void draw() const {
		if (isCreated()) {
			m_items[m_curr_draw_item].draw();
		}
	}
	
	bool isCreated() const { return m_items != NULL; }

	void toggleDisplist();
	void toggleDisplist(GLuint ind);

	GLuint getCurrentObjectInd() const { 
		return m_curr_draw_item;
	}
	
	GLuint getObjectCount() const { return m_item_count; }
};

#endif


