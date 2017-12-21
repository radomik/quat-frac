#include "stdafx.h"
#include "GLdrawing.hpp"

GLdrawingItem* GLdrawing::getItem(GLuint index) {
	const char* self_name = "GLdrawing::getItem:";
	if (!m_items) {
		fprintf(stderr, "%s m_items array is NULL (destroyed or missing init())\n",
			self_name);
		return NULL;
	}
	if (index >= m_item_count) {
		fprintf(stderr, "%s Invalid index %u, while item_count = %u\n",
			self_name, index, m_item_count);
		return NULL;
	}
	return &m_items[index];
}

int GLdrawing::init(GLuint count_display_objects) {
	const char *self_name = "GLdrawing::getItem:";
	if (!count_display_objects) {
		fprintf(stderr, "%s Cannot initialize to 0 items\n", self_name);
		return -1;
	}
	if (m_items) {
		fprintf(stderr, "%s WARNING: Deleting current array of objects before initialization\n", self_name);
		//fprintf(stderr, "[DBG] %s @ %d [DEL] %p (GLdrawingItem[] m_items)\n", __FILE__, __LINE__, m_items);
		delete[] m_items;
	}
	
	try {
		m_items = new GLdrawingItem[count_display_objects];
		//fprintf(stderr, "[DBG] %s @ %d [NEW] %p (GLdrawingItem[] m_items)\n", __FILE__, __LINE__, m_items);
	}
	catch (const std::bad_alloc& e) {
		SysError ex;
		fprintf(stderr, "%s Failed to allocate GLdrawingItem[%u] [what: %s, errno: %s]\n", self_name, count_display_objects, e.what(), ex.msg);
		return -1;
	}
	
	m_item_count = count_display_objects;
	
	return 0;
}

void GLdrawing::toggleDisplist() {
	if (isCreated()) {
		m_curr_draw_item++;
		m_curr_draw_item %= m_item_count;
	}
}

void GLdrawing::toggleDisplist(GLuint ind) {
	if (ind < m_item_count) {
		m_curr_draw_item = ind;
	}
}
