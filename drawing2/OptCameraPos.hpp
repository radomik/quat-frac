/*
* File:   OptCameraPos.hpp
* Author: darek
*
* Created on 23 November 2014, 22:43
*/
#ifndef QFRACTAL_DRAWING_OPTCAMERAPOS_HPP
#define QFRACTAL_DRAWING_OPTCAMERAPOS_HPP

#include "ISerializable.hpp"
#include "utils.hpp"
#include "Vec.hpp"

class OptCameraPos : public ISerializable {
public:
	Vec3f	pos;
	Vec3f	target;
	Vec3f	up;

	void setDefault();

	virtual int readText(Parser& parser);
	virtual int saveText(Parser& parser) const;

	virtual void readBin(FILE* f);
	virtual void saveBin(FILE* f) const;
};

#endif
