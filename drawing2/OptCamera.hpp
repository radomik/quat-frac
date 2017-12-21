/*
* File:   OptCamera.hpp
* Author: darek
*
* Created on 23 November 2014, 22:49
*/
#ifndef QFRACTAL_DRAWING_OPTCAMERA_HPP
#define QFRACTAL_DRAWING_OPTCAMERA_HPP

#include "ISerializable.hpp"
#include "utils.hpp"

class OptCameraTransSpeed : public ISerializable {
public:
	float left_right; // above zero
	float up_down;    // above zero

	void	setDefault();

	virtual int readText(Parser& parser);
	virtual int saveText(Parser& parser) const;

	virtual void readBin(FILE* f);
	virtual void saveBin(FILE* f) const;
};

class OptCameraZoomSpeed : public ISerializable {
public:
	float without_ctrl; // above zero
	float with_ctrl;    // above zero

	void	setDefault();

	virtual int readText(Parser& parser);
	virtual int saveText(Parser& parser) const;

	virtual void readBin(FILE* f);
	virtual void saveBin(FILE* f) const;
};

class OptCameraRotate : public ISerializable {
public:
	float						sensitivity;
	float						speed;
	bool						enabled_mouse_rotate;

	void	setDefault();

	virtual int readText(Parser& parser);
	virtual int saveText(Parser& parser) const;

	virtual void readBin(FILE* f);
	virtual void saveBin(FILE* f) const;
};

class OptCamera : public ISerializable {
public:
	OptCameraTransSpeed		trans_speed;
	OptCameraZoomSpeed		zoom_speed;
	OptCameraRotate			rotate;

	void	setDefault();

	virtual int readText(Parser& parser);
	virtual int saveText(Parser& parser) const;

	virtual void readBin(FILE* f);
	virtual void saveBin(FILE* f) const;
};

#endif
