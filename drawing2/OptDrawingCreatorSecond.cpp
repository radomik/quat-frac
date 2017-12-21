#include "stdafx.h"
#include "OptDrawingCreatorSecond.hpp"
#include "Parser.hpp"
#include "BinSerialization.hpp"
#include "Integer.hpp"
#include "Quat.hpp"
#include "Name.hpp"

void OptDrawingCreatorSecond::customSet(const OptDrawingCreator &other) {
	const OptDrawingCreatorSecond *other_casted = 
		dynamic_cast< const OptDrawingCreatorSecond* >(&other);
	assert(other_casted != NULL);
	
	coord_map = other_casted->coord_map;
	color_multiplier = other_casted->color_multiplier;
}

void OptDrawingCreatorSecond::customSetDefault() {
	coord_map[0] = QuatIndX;
	coord_map[1] = QuatIndY;
	coord_map[2] = QuatIndZ;
	color_multiplier = 4;
}

int OptDrawingCreatorSecond::customReadText(Parser &parser) {
	const char *self_name = "OptDrawingCreatorSecond";
	
	U32 	o_color_multiplier;

	bool rd_coord_map = false;
	bool rd_color_multiplier = false;
	
	int ret;
	
	while (! parser.isEof()) {
		if (parser.parseLine() < 0)
			return -1;
		
		if (parser.isClosureEnd()) break;
		
		if (parser.isClosureBegin()) {
			// omit all other closures (may belong to base OptDrawingCreator class)
			if (parser.omitClosure()) return -1;
		}
		else {
			if (parser.isToken()) {
				if ((ret = parser.parseToken(coord_map, Name::coord_map, &rd_coord_map, self_name)) < 0) return -1;
				if ((ret > 0) && (rd_coord_map)) { continue; }
				
				if ((ret = parser.parseToken(o_color_multiplier, Name::color_multiplier, &rd_color_multiplier, self_name)) < 0) return -1;
				if ((ret > 0) && (rd_color_multiplier)) { color_multiplier = o_color_multiplier.value; continue; }
				
				// omit all other tokens (may belong to base OptDrawingCreator class)
			}
		}
	}
	
	if ((parser.isFull()) && (! (rd_coord_map && rd_color_multiplier)))
	{
		fprintf(stderr, "%s: Partially parsed:\n\
Have{coord_map,color_multiplier}={%d,%d}\n", self_name, 
rd_coord_map, rd_color_multiplier);
		return -1;
	}
	
	return 0;
}

int OptDrawingCreatorSecond::customSaveText(Parser &parser) const {
	U32 	o_color_multiplier;
	
	if (parser.writePrimitive(coord_map, Name::coord_map) < 0) return -1;
	
	o_color_multiplier.value = color_multiplier;
	if (parser.writePrimitive(o_color_multiplier, Name::color_multiplier) < 0) return -1;
	
	
	return 0;
}

int OptDrawingCreatorSecond::customReadBin(FILE *f) {
	if (coord_map.readBin(f) < 0) return -1;
	if (BinSerialization::readValue<unsigned int>(f, color_multiplier) < 0) return -1;
	
	return 0;
}

int OptDrawingCreatorSecond::customSaveBin(FILE *f) const {
	if (coord_map.saveBin(f) < 0) return -1;
	if (BinSerialization::writeValue<unsigned int>(f, color_multiplier) < 0) return -1;
	
	return 0;
}

