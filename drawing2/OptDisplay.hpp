/*
 * File:   OptDisplay.hpp
 * Author: darek
 *
 * Created on 23 November 2014, 23:27
 */
#ifndef QFRACTAL_DRAWING_OPTDISPLAY_HPP
#define QFRACTAL_DRAWING_OPTDISPLAY_HPP

#include "ISerializable.hpp"
#include "utils.hpp"
#include "Color.hpp"

class OptDisplayGeom : public ISerializable {
public:
	unsigned int x, y, w, h;

	void	setDefault();

	virtual int readText(Parser& parser);
	virtual int saveText(Parser& parser) const;

	virtual void readBin(FILE* f);
	virtual void saveBin(FILE* f) const;
};

class OptDisplay : public ISerializable {
public:
	Color4f			background;
	OptDisplayGeom	disp_geom;

	void	setDefault();

	virtual int readText(Parser& parser);
	virtual int saveText(Parser& parser) const;

	virtual void readBin(FILE* f);
	virtual void saveBin(FILE* f) const;

	void apply() {
		_setBackground();
	}

private:
	void _setBackground();
};

#endif

