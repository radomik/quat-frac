#include "stdafx.h"
#include "OptDrawingCreator.hpp"
#include "Parser.hpp"
#include "BinSerialization.hpp"
#include "Integer.hpp"
#include "Name.hpp"
#include "OptDrawingCreatorFirst.hpp"
#include "OptDrawingCreatorSecond.hpp"
#include "TwoStateVariable.hpp"

void OptDrawingCreator::set(const OptDrawingCreator &other) {
	assert(this->version == other.version);
	
	disp_list_ind = other.disp_list_ind;
	color = other.color;
	color_negative = other.color_negative;
	
	customSet(other);
}

void OptDrawingCreator::setDefault() {
	disp_list_ind = 0;
	color_negative = false;
	color.r() = 0.3f;
	color.g() = 0.3f;
	color.b() = 0.3f;
	color.a() = 0.1f;

	customSetDefault();
}

/// read common fields
/// @note version is already read
/// @see readNewByText
int OptDrawingCreator::_readTextCommon(Parser &parser) 
{
	const char *self_name = "OptDrawingCreator";
	
	U32   	val_uint;
	YesNo	o_color_negative;
	
	bool rd_disp_list_ind = false;
	bool rd_color = false;
	bool rd_color_negative = false;
	
	int ret;
	
	while (! parser.isEof()) {
		if (parser.parseLine() < 0)
			return -1;
		
		if (parser.isClosureEnd()) break;
		
		if (parser.isClosureBegin()) {
			if ((ret = parser.parseClosure(color, Name::color, &rd_color, self_name)) < 0) return -1;
			if ((ret > 0) && (rd_color)) continue;

			// omit all other closures (may belong to classes derived from OptDrawingCreator)
			if (parser.omitClosure()) return -1; 
		}
		else {
			if (parser.isToken()) {
				if ((ret=parser.parseToken(val_uint, Name::disp_list_ind, &rd_disp_list_ind, self_name)) < 0) return -1;
				if ((ret > 0) && (rd_disp_list_ind)) { disp_list_ind = val_uint.value; continue; }
				
				if ((ret = parser.parseToken(o_color_negative, Name::color_negative, &rd_color_negative, self_name)) < 0) return -1;
				if ((ret > 0) && (rd_color_negative)) { color_negative = o_color_negative.value; continue; }

				// omit all other tokens (may belong to classes derived from OptGenerator)
			}
		}
	}
	
	if ((parser.isFull()) && (!(rd_disp_list_ind && rd_color && rd_color_negative)))
	{
		fprintf(stderr, "%s: Partially parsed:\n\
Have{disp_list_ind,color,color_negative}={%d,%d,%d}\n", self_name, rd_disp_list_ind, rd_color, rd_color_negative);
		return -1;
	}
	
	return 0;
}

// read common + custom fields
int OptDrawingCreator::readText(Parser &parser) 
{
	const char *self_name = "OptDrawingCreator::readText:";
	
	// store current parser position
	ParserPosition pos;
	if (parser.getPosition(pos) < 0) {
		fprintf(stderr, "%s Failed to store Parser position\n", self_name);
		return -1;
	}
	
	// read fields common to all classes derived from OptDrawingCreator
	if (_readTextCommon(parser) < 0) {
		fprintf(stderr, "%s Failed to read common fields\n", self_name);
		return -1;
	}
	
	// restore parser position
	// cloures and tokens belonging to OptDrawingCreator and child class may be mixed
	// in text file
	if (parser.setPosition(pos) < 0) {
		fprintf(stderr, "%s Failed to restore Parser position\n", self_name);
		return -1;
	}
	
	// read child class fields
	if (customReadText(parser) < 0) {
		fprintf(stderr, "%s Failed to read custom fields\n", self_name);
		return -1;
	}
	
	return 0;
}

/// read common fields
/// @note version is already read
/// @see readNewByBin()
int OptDrawingCreator::_readBinCommon(FILE *f) 
{
	if (color.readBin(f) < 0) return -1;
	if (BinSerialization::readValue<unsigned int>(f, disp_list_ind) < 0) return -1;
	if (BinSerialization::readBool(f, color_negative) < 0) return -1;
	
	return 0;
}

// read common + custom fields
int OptDrawingCreator::readBin(FILE *f) 
{
	const char *self_name = "OptDrawingCreator::readBin:";
	
	// read fields common to all classes derived from OptDrawingCreator
	if (_readBinCommon(f) < 0) {
		fprintf(stderr, "%s Failed to read common fields\n", self_name);
		return -1;
	}
	
	// read child class fields
	if (customReadBin(f) < 0) {
		fprintf(stderr, "%s Failed to read custom fields\n", self_name);
		return -1;
	}
	
	return 0;
}

/// save common fields (also version)
int OptDrawingCreator::_saveTextCommon(Parser &parser) const {
	U32   	val_uint;
	YesNo	o_color_negative;
	
	val_uint.value = version;
	if (parser.writePrimitive(val_uint, Name::version) < 0) return -1;
	
	if (parser.writeObject(color, Name::color) < 0) return -1;

	o_color_negative.value = color_negative;
	if (parser.writePrimitive(o_color_negative, Name::color_negative) < 0) return -1;

	val_uint.value = disp_list_ind;
	if (parser.writePrimitive(val_uint, Name::disp_list_ind) < 0) return -1;
	
	return 0;
}

// save common + custom fields
int OptDrawingCreator::saveText(Parser &parser) const 
{
	// save fields common to all classes derived from OptDrawingCreator
	if (_saveTextCommon(parser) < 0) return -1;
	
	// save child class fields
	return customSaveText(parser);
}

/// save common fields (also version)
int OptDrawingCreator::_saveBinCommon(FILE *f) const
{
	if (BinSerialization::writeValueAsOneByte<unsigned int>(f, version) < 0) return -1;
	if (color.saveBin(f) < 0) return -1;
	if (BinSerialization::writeValue<unsigned int>(f, disp_list_ind) < 0) return -1;
	if (BinSerialization::writeBool(f, color_negative) < 0) return -1;

	return 0;
}

// save common + custom fields
int OptDrawingCreator::saveBin(FILE *f) const {
	// save fields common to all classes derived from OptDrawingCreator
	if (_saveBinCommon(f) < 0) return -1;
	
	// save child class fields
	return customSaveBin(f);
}

// Create new custom object with read all fields
OptDrawingCreator* OptDrawingCreator::readNewByText(Parser &parser) {
	const char *self_name = "OptDrawingCreator::readText";
	
	U32  o_version;
	int  ret;
	
	while (! parser.isEof()) {
		if (parser.parseLine() < 0)
			return NULL;
		
		if (parser.isClosureEnd()) break;
		
		if (parser.isClosureBegin()) {
			_st_unexpectedItem(parser.getId(), "closure");
			return NULL;
		}
		else {
			if (parser.isToken()) {
				// read version token (must be first in opt_drawing closure)
				if ((ret=parser.parseToken(o_version, Name::version, NULL, self_name)) < 0) return NULL;
				if (ret > 0) {
					// read OptDrawingCreator of appropiate type from obtained version
					return _st_readText(parser, o_version.value);
				}
				
				_st_unexpectedItem(parser.getId(), "token");
				return NULL;
			}
		}
	}
	
	fprintf(stderr, "OptDrawingCreator::readText: [ERROR] Token 'version' not found (inside opt_drawing_creator)\n");
	return NULL;
}

// Create new custom object with read all fields
OptDrawingCreator* OptDrawingCreator::readNewByBin(FILE *f) {
	unsigned int version;
	
	if (BinSerialization::readValueAsOneByte<unsigned int>(f, version) < 0) {
		fprintf(stderr, "OptDrawingCreator::readBin: Failed to read version\n");
		return NULL;
	}
	
	OptDrawingCreator* draw = newByVersion(version);
	
	if (! draw) {
		fprintf(stderr, "OptDrawingCreator::readBin: Failed to create opt_drawing of version: %u\n", version);
		return NULL;
	}
	
	if (! draw->readBin(f)) { // call to OptDrawingCreator::readBin()
		fprintf(stderr, "OptDrawingCreator::readBin: Failed to readBin of version: %u\n", version);
		
		//fprintf(stderr, "[DBG] %s @ %d [DEL] %p (OptDrawingCreator)\n", __FILE__, __LINE__, draw);
		delete(draw);
		return NULL;
	}
	
	return draw;
}

OptDrawingCreator* OptDrawingCreator::newByCopy(const OptDrawingCreator &other) {
	OptDrawingCreator* draw = newByVersion(other.version);
	if (draw) draw->set(other);
	return draw;
}

void OptDrawingCreator::_st_unexpectedItem(const char *id, const char *id_type) {
	fprintf(stderr, "OptDrawingCreator: Unexpected %s: '%s', \
expected to find token 'version' at first position in opt_drawing closure\n", 
		id_type, id);
}

OptDrawingCreator* OptDrawingCreator::_st_readText(Parser &parser, unsigned int version) {
	OptDrawingCreator* draw = newByVersion(version);
	if (! draw) return NULL;
	
	if (draw->readText(parser) < 0) { // call to OptDrawingCreator::readText()
		fprintf(stderr, "OptDrawingCreator::_st_readText: Failed to read readText of version %u\n",
			version);
			
		//fprintf(stderr, "[DBG] %s @ %d [DEL] %p (OptDrawingCreator)\n", __FILE__, __LINE__, draw);
		delete(draw);
		return NULL;
	}
	
	return draw;
}

OptDrawingCreator* OptDrawingCreator::newByVersion(unsigned int version) {
	OptDrawingCreator *dw;
	switch (version) {
		case 1: dw = new OptDrawingCreatorFirst(); break;
		case 2: dw = new OptDrawingCreatorSecond(); break;
		default:
			fprintf(stderr, "OptDrawingCreator: Invalid version: %u (expected: 1 or 2)\n", version);
			return NULL;
	}
	
	//fprintf(stderr, "[DBG] %s @ %d [NEW] %p (OptDrawingCreator)\n", __FILE__, __LINE__, dw);
	
	dw->version = version;
	return dw;
}
