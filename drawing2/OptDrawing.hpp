/*
* File:   OptDrawing.hpp
* Author: darek
*
* Created on 18 December 2014, 01:06
*/
#ifndef QFRACTAL_DRAWING_OPTDRAWING_HPP
#define QFRACTAL_DRAWING_OPTDRAWING_HPP

#include "ISerializable.hpp"
#include "utils.hpp"
#include "DrawMode.hpp"
#include "Vec.hpp"

class OptDrawing : public ISerializable {
public:
	Vec3f			rotation;
	draw_mode_t		draw_mode;
	float			rotate_angle;

	void	setDefault();

	virtual int readText(Parser& parser);
	virtual int saveText(Parser& parser) const;

	virtual void readBin(FILE* f);
	virtual void saveBin(FILE* f) const;

	void apply() {
		_setDrawMode();
	}

	void toggleDrawMode();

private:
	void _setDrawMode();
};

#endif

