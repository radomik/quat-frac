/*
* File:   Name.hpp
* Author: darek
*
* Created on 21 November 2014, 01:53
*/
#ifndef QFRACTAL_DRAWING_NAME_HPP
#define QFRACTAL_DRAWING_NAME_HPP

#include "utils.hpp"

/// cat names.txt | sort | uniq | awk '{ print "\tstatic const char* " $1 ";" }'
class Name {
public:
	static const char* a;
	static const char* alpha_2;
	static const char* b;
	static const char* background;
	static const char* buf_size;
	static const char* color;
	static const char* color_multiplier;
	static const char* color_negative;
	static const char* color_negative_2;
	static const char* coord_map;
	static const char* disp_geom;
	static const char* disp_list_ind;
	static const char* draw_mode;
	static const char* elem_size_percent;
	static const char* enabled_mouse_rotate;
	static const char* fun_index;
	static const char* g;
	static const char* h;
	static const char* initial;
	static const char* iter;
	static const char* left_right;
	static const char* limit;
	static const char* max;
	static const char* max_len_sqr;
	static const char* maxy;
	static const char* min;
	static const char* miny;
	static const char* mod_q;
	static const char* mul_range_by_pi;
	static const char* opt_app_drawing;
	static const char* opt_camera;
	static const char* opt_camera_pos;
	static const char* opt_display;
	static const char* opt_drawing;
	static const char* opt_drawing_creator;
	static const char* opt_generator;
	static const char* pos;
	static const char* r;
	static const char* range;
	static const char* rotate;
	static const char* rotate_angle;
	static const char* rotation;
	static const char* sensitivity;
	static const char* speed;
	static const char* step;
	static const char* target;
	static const char* trans_speed;
	static const char* up;
	static const char* up_down;
	static const char* version;
	static const char* w;
	static const char* win_size;
	static const char* with_ctrl;
	static const char* without_ctrl;
	static const char* x;
	static const char* y;
	static const char* z;
	static const char* zoom_speed;

	static const char* _rgba[4];
	static const char* _xyzw[4];
	static const char* _xyz[3];
};

#endif
