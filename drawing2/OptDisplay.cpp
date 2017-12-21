#include "stdafx.h"
#include "OptDisplay.hpp"
#include "FloatingPoint.hpp"
#include "Parser.hpp"
#include "BinSerialization.hpp"
#include "Name.hpp"
#include "Integer.hpp"
#include "gfx.hpp"

void OptDisplayGeom::setDefault() {
	x = 0;
	y = 0;
	w = 800;
	h = 800;
}

void OptDisplay::setDefault() {
	background.r() = 1.0f;
	background.g() = 1.0f;
	background.b() = 1.0f;
	background.a() = 0.75f;
	disp_geom.setDefault();
}

int OptDisplayGeom::readText(Parser& parser) {
	const char *self_name = "OptDisplayGeom";
	
	U32	val_uint;
	
	bool rd_x	 = false;
	bool rd_y	 = false;
	bool rd_w	 = false;
	bool rd_h	 = false;
	
	int ret;
	
	while (! parser.isEof()) {
		if (parser.parseLine() < 0)
			return -1;
		
		if (parser.isClosureEnd()) break;
		
		if (parser.isClosureBegin()) {
			parser.unexpectedClosure(self_name);
			return -1;
		}
		else {
			if (parser.isToken()) {
				if ((ret=parser.parseToken(val_uint, Name::x, &rd_x, self_name)) < 0) return -1;
				if ((ret > 0) && (rd_x)) { x = val_uint.value; continue; }
				
				if ((ret=parser.parseToken(val_uint, Name::y, &rd_y, self_name)) < 0) return -1;
				if ((ret > 0) && (rd_y)) { y = val_uint.value; continue; }
				
				if ((ret=parser.parseToken(val_uint, Name::w, &rd_w, self_name)) < 0) return -1;
				if ((ret > 0) && (rd_w)) { w = val_uint.value; continue; }
				
				if ((ret=parser.parseToken(val_uint, Name::h, &rd_h, self_name)) < 0) return -1;
				if ((ret > 0) && (rd_h)) { h = val_uint.value; continue; }
				
				parser.unexpectedToken(self_name);
				return -1;
			}
		}
	}
	
	if ((parser.isFull()) && (! (rd_x && rd_y && rd_w && rd_h)))
	{
		fprintf(stderr, "%s: Partially parsed:\n\
Have{x,y ; w,h}={%d,%d ; %d,%d}\n", self_name,
			rd_x, rd_y, rd_w, rd_h);
		return -1;
	}
	
	return 0;
}
	
int OptDisplay::readText(Parser& parser) {
	const char *self_name = "OptDisplay";

	Float o_rotate_angle;
	
	bool rd_background	 = false;
	bool rd_disp_geom	 = false;
	
	int ret;
	
	while (! parser.isEof()) {
		if (parser.parseLine() < 0)
			return -1;
		
		if (parser.isClosureEnd()) break;
		
		if (parser.isClosureBegin()) {
			if ((ret=parser.parseClosure(background, Name::background, &rd_background, self_name)) < 0) return -1;
			if ((ret > 0) && (rd_background)) continue;
			
			if ((ret=parser.parseClosure(disp_geom, Name::disp_geom, &rd_disp_geom, self_name)) < 0) return -1;
			if ((ret > 0) && (rd_disp_geom)) continue;

			parser.unexpectedClosure(self_name);
			return -1;
		}
		else {
			if (parser.isToken()) {
				parser.unexpectedToken(self_name);
				return -1;
			}
		}
	}
	
	if ((parser.isFull()) && (! (rd_background && rd_disp_geom)))
	{
		fprintf(stderr, "%s: Partially parsed:\n\
Have{background ; disp_geom}={%d,%d}\n", self_name,
			rd_background, rd_disp_geom);
		return -1;
	}
	
	return 0;
}

int OptDisplayGeom::saveText(Parser& parser) const {
	U32 val_uint;
	
	val_uint.value = x;
	if (parser.writePrimitive(val_uint, Name::x) < 0) return -1;
	
	val_uint.value = y;
	if (parser.writePrimitive(val_uint, Name::y) < 0) return -1;
	
	val_uint.value = w;
	if (parser.writePrimitive(val_uint, Name::w) < 0) return -1;
	
	val_uint.value = h;
	if (parser.writePrimitive(val_uint, Name::h) < 0) return -1;
	
	return 0;
}

int OptDisplay::saveText(Parser& parser) const {
	Float o_rotate_angle;
	
	if (parser.writeObject(background, Name::background) < 0) return -1;
	if (parser.writeObject(disp_geom, Name::disp_geom) < 0) return -1;
	
	return 0;
}

int OptDisplayGeom::readBin(FILE* f) {
	if (BinSerialization::readValue<unsigned int>(f, x) < 0) return -1;
	if (BinSerialization::readValue<unsigned int>(f, y) < 0) return -1;
	if (BinSerialization::readValue<unsigned int>(f, w) < 0) return -1;
	if (BinSerialization::readValue<unsigned int>(f, h) < 0) return -1;
	
	return 0;
}

int OptDisplay::readBin(FILE* f) {
	if (background.readBin(f) < 0) return -1;
	if (disp_geom.readBin(f) < 0) return -1;
	return 0;
}

int OptDisplay::saveBin(FILE* f) const {
	if (background.saveBin(f) < 0) return -1;
	if (disp_geom.saveBin(f) < 0) return -1;
	return 0;
}

int OptDisplayGeom::saveBin(FILE* f) const {
	if (BinSerialization::writeValue<unsigned int>(f, x) < 0) return -1;
	if (BinSerialization::writeValue<unsigned int>(f, y) < 0) return -1;
	if (BinSerialization::writeValue<unsigned int>(f, w) < 0) return -1;
	if (BinSerialization::writeValue<unsigned int>(f, h) < 0) return -1;
	
	return 0;
}

void OptDisplay::_setBackground() {
	glClearColor(background.rConst(), background.gConst(), background.bConst(), background.aConst());
}
