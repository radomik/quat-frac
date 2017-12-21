#include "stdafx.h"
#include "OptCameraPos.hpp"
#include "Parser.hpp"
#include "BinSerialization.hpp"
#include "Name.hpp"

void OptCameraPos::setDefault() {
	pos.set(-0.231f, 1.892f, 8.410f);
	target.set(-0.462f, 1.907f, -2.587f);
	up.set(0.0f, 1.0f, 0.0f);
}

int OptCameraPos::readText(Parser& parser) {
	const char *self_name = "OptCameraPos";
	
	bool rd_pos		= false;
	bool rd_target	= false;
	bool rd_up		= false;
	
	int ret;
	
	while (! parser.isEof()) {
		if (parser.parseLine() < 0)
			return -1;
		
		if (parser.isClosureEnd()) break;
		
		if (parser.isClosureBegin()) {
			if ((ret=parser.parseClosure(pos, Name::pos, &rd_pos, self_name)) < 0) return -1;
			if ((ret > 0) && (rd_pos)) continue;
			
			if ((ret=parser.parseClosure(target, Name::target, &rd_target, self_name)) < 0) return -1;
			if ((ret > 0) && (rd_target)) continue;
			
			if ((ret=parser.parseClosure(up, Name::up, &rd_up, self_name)) < 0) return -1;
			if ((ret > 0) && (rd_up)) continue;
			
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
	
	if ((parser.isFull()) && (! (rd_pos && rd_target && rd_up)))
	{
		fprintf(stderr, "%s: Partially parsed:\n\
Have{pos,target,up}={%d,%d,%d}\n", self_name, rd_pos, rd_target, rd_up);
		return -1;
	}
	
	return 0;
}

int OptCameraPos::saveText(Parser& parser) const {
	if (parser.writeObject(pos, Name::pos) < 0) return -1;
	if (parser.writeObject(target, Name::target) < 0) return -1;
	if (parser.writeObject(up, Name::up) < 0) return -1;
	
	return 0;
}

int OptCameraPos::readBin(FILE* f) {
	if (pos.readBin(f)) return -1;
	if (target.readBin(f)) return -1;
	if (up.readBin(f)) return -1;
	return 0;
}

int OptCameraPos::saveBin(FILE* f) const {
	if (pos.saveBin(f)) return -1;
	if (target.saveBin(f)) return -1;
	if (up.saveBin(f)) return -1;
	return 0;
}
