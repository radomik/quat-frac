#include "stdafx.h"
#include "OptDrawingCreatorFirst.hpp"
#include "Parser.hpp"
#include "BinSerialization.hpp"
#include "Name.hpp"
#include "FloatingPoint.hpp"
#include "TwoStateVariable.hpp"

#define DEFAULT_ELEM_SIZE_PERCENT	100.0f
#define DEFAULT_ALPHA_2 			0.2f
#define DEFAULT_COLOR_NEGATIVE_2 	true

void OptDrawingCreatorFirst::customSet(const OptDrawingCreator &other) {
	const OptDrawingCreatorFirst *other_casted = 
		dynamic_cast< const OptDrawingCreatorFirst* >(&other);
	assert(other_casted != NULL);
	
	elem_size_percent = other_casted->elem_size_percent;
	alpha_2 = other_casted->alpha_2;
	color_negative_2 = other_casted->color_negative_2;
}

void OptDrawingCreatorFirst::customSetDefault() {
	elem_size_percent = DEFAULT_ELEM_SIZE_PERCENT;
	alpha_2 = DEFAULT_ALPHA_2;
	color_negative_2 = DEFAULT_COLOR_NEGATIVE_2;
}

int OptDrawingCreatorFirst::customReadText(Parser &parser) {
	const char *self_name = "OptDrawingCreatorFirst";
	
	Float	val_float;
	YesNo	val_yesno;
	bool 	rd_elem_size_percent = false;
	bool 	rd_alpha_2 = false;
	bool 	rd_color_negative_2 = false;
	int 	ret;
	
	while (! parser.isEof()) {
		if (parser.parseLine() < 0)
			return -1;

		//fprintf(stderr, "%s: parser.line: '%s'\n", self_name, parser.getLine());
		//fprintf(stderr, "%s: parser.id: %s\n", self_name, parser.getId());
		
		if (parser.isClosureEnd()) break;
		
		if (parser.isClosureBegin()) {
			// omit all other closures (may belong to base OptDrawingCreator class)
			if (parser.omitClosure()) return -1; 
		}
		else {
			if (parser.isToken()) {

				if ((ret = parser.parseToken(val_float, Name::elem_size_percent, &rd_elem_size_percent, self_name)) < 0) return -1;
				if ((ret > 0) && (rd_elem_size_percent)) { 
					elem_size_percent = val_float.value; continue; }
				
				if ((ret = parser.parseToken(val_float, Name::alpha_2, &rd_alpha_2, self_name)) < 0) return -1;
				if ((ret > 0) && (rd_alpha_2)) { 
					alpha_2 = val_float.value; continue; }
					
				if ((ret = parser.parseToken(val_yesno, Name::color_negative_2, &rd_color_negative_2, self_name)) < 0) return -1;
				if ((ret > 0) && (rd_color_negative_2)) { 
					color_negative_2 = val_yesno.value; continue; }
				
				// omit all other tokens (may belong to base OptDrawingCreator class)
			}
		}
	}
	
	if (! rd_alpha_2) alpha_2 = DEFAULT_ALPHA_2;
	if (! rd_color_negative_2) color_negative_2 = DEFAULT_COLOR_NEGATIVE_2;
	
	if ((parser.isFull()) && (! (rd_elem_size_percent)))
	{
		fprintf(stderr, "%s: Partially parsed:\n\
Have{elem_size_percent}={%d}\n", self_name, rd_elem_size_percent);
		return -1;
	}
	
	if (Float::cmp(elem_size_percent, 0.0f) <= 0) {
		fprintf(stderr, "%s: Invalid elem_size_percent = %f (expected value above zero)\n", self_name, elem_size_percent);
		return -1;
	}
	
	return 0;
}

int OptDrawingCreatorFirst::customSaveText(Parser &parser) const {
	Float 	val_float;
	YesNo	val_yesno;
	
	val_float.value = elem_size_percent;
	if (parser.writePrimitive(val_float, Name::elem_size_percent) < 0) return -1;
	
	val_float.value = alpha_2;
	if (parser.writePrimitive(val_float, Name::alpha_2) < 0) return -1;
	
	val_yesno.value = color_negative_2;
	if (parser.writePrimitive(val_yesno, Name::color_negative_2) < 0) return -1;
	
	return 0;
}

int OptDrawingCreatorFirst::customReadBin(FILE *f) {
	if (BinSerialization::readValue<float>(f, elem_size_percent) < 0) return -1;
	if (BinSerialization::readValue<float>(f, alpha_2) < 0) return -1;
	if (BinSerialization::readBool(f, color_negative_2) < 0) return -1;
	
	return 0;
}

int OptDrawingCreatorFirst::customSaveBin(FILE *f) const {
	if (BinSerialization::writeValue<float>(f, elem_size_percent) < 0) return -1;
	if (BinSerialization::writeValue<float>(f, alpha_2) < 0) return -1;
	if (BinSerialization::writeBool(f, color_negative_2) < 0) return -1;
	
	return 0;
}
