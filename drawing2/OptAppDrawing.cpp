#include "stdafx.h"
#include "OptAppDrawing.hpp"
#include "Parser.hpp"
#include "Name.hpp"

OptAppDrawing::~OptAppDrawing() {
	if (opt_drawing_creator) {
		//fprintf(stderr, "[DBG] %s @ %d [DEL] %p (OptDrawingCreator)\n", __FILE__, __LINE__, opt_drawing_creator);
		delete(opt_drawing_creator);
		opt_drawing_creator = NULL;
	}
}

int OptAppDrawing::setDefault(unsigned int version) {
	opt_camera.setDefault();
	opt_camera_pos.setDefault();
	opt_display.setDefault();
	opt_drawing.setDefault();
	
	if (opt_drawing_creator) {
		if (opt_drawing_creator->version != version) { // create new only if version mismatch
			//fprintf(stderr, "[DBG] %s @ %d [DEL] %p (OptDrawingCreator)\n", __FILE__, __LINE__, opt_drawing_creator);
			delete(opt_drawing_creator);
			if ((opt_drawing_creator = OptDrawingCreator::newByVersion(version)) == NULL) return -1;
			opt_drawing_creator->setDefault();
			return 0;
		}
	}
	else { // create new opt_drawing
		if ((opt_drawing_creator = OptDrawingCreator::newByVersion(version)) == NULL) return -1;
	}
	
	opt_drawing_creator->setDefault();
	return 0;
}

int OptAppDrawing::readText(Parser& parser) {
	const char *self_name = "OptAppDrawing";
	
	bool rd_opt_camera			= false;
	bool rd_opt_camera_pos		= false;
	bool rd_opt_display			= false;
	bool rd_opt_drawing			= false;
	bool rd_opt_drawing_creator = false;
	
	int ret;
	
	if (opt_drawing_creator) { 
		//fprintf(stderr, "[DBG] %s @ %d [DEL] %p (OptDrawingCreator)\n", __FILE__, __LINE__, opt_drawing_creator);
		delete(opt_drawing_creator);
		opt_drawing_creator = NULL;
	}
	
	while (! parser.isEof()) {
		if (parser.parseLine() < 0)
			return -1;
		
		if (parser.isClosureEnd()) break;
		
		if (parser.isClosureBegin()) {
			if ((ret=parser.parseClosure(opt_camera, Name::opt_camera, &rd_opt_camera, self_name)) < 0) return -1;
			if ((ret > 0) && (rd_opt_camera)) continue;
			
			if ((ret=parser.parseClosure(opt_camera_pos, Name::opt_camera_pos, &rd_opt_camera_pos, self_name)) < 0) return -1;
			if ((ret > 0) && (rd_opt_camera_pos)) continue;
			
			if ((ret=parser.parseClosure(opt_display, Name::opt_display, &rd_opt_display, self_name)) < 0) return -1;
			if ((ret > 0) && (rd_opt_display)) continue;
			
			if ((ret = parser.parseClosure(opt_drawing, Name::opt_drawing, &rd_opt_drawing, self_name)) < 0) return -1;
			if ((ret > 0) && (rd_opt_drawing)) continue;


			if (! parser.cmpId(Name::opt_drawing_creator)) {
				if (opt_drawing_creator != NULL) {
					fprintf(stderr, "%s: .%s already read\n", self_name, Name::opt_drawing_creator);
					return -1;
				}
				
				if ((opt_drawing_creator = OptDrawingCreator::readNewByText(parser)) == NULL) {
					fprintf(stderr, "%s: Failed to read .%s\n", self_name, Name::opt_drawing_creator);
					return -1;
				}
				
				rd_opt_drawing_creator = true;
				continue;
			}
			
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
	
	if ((parser.isFull()) && (! (rd_opt_camera && rd_opt_camera_pos && rd_opt_display && rd_opt_drawing && rd_opt_drawing_creator)))
	{
		fprintf(stderr, "%s: Partially parsed:\n\
Have{opt_camera,opt_camera_pos,opt_display ; opt_drawing,opt_drawing_creator}=\n\
{%d,%d,%d ; %d,%d}\n", self_name,
			rd_opt_camera, rd_opt_camera_pos, rd_opt_display, rd_opt_drawing, rd_opt_drawing_creator);
		return -1;
	}
	
	return 0;
}

int OptAppDrawing::saveText(Parser& parser) const {
	// write: opt_camera
	if (parser.writeObject(opt_camera, Name::opt_camera) < 0) return -1;
	
	// write: opt_camera_pos
	if (parser.writeObject(opt_camera_pos, Name::opt_camera_pos) < 0) return -1;
	
	// write: opt_display
	if (parser.writeObject(opt_display, Name::opt_display) < 0) return -1;
	
	// write: opt_drawing
	if (parser.writeObject(opt_drawing, Name::opt_drawing) < 0) return -1;

	// write: opt_drawing_creator
	assert(opt_drawing_creator != NULL);
	return parser.writeObject(*opt_drawing_creator, Name::opt_drawing_creator);
}

int OptAppDrawing::readBin(FILE* f) {
	if (opt_camera.readBin(f) < 0) return -1;
	if (opt_camera_pos.readBin(f) < 0) return -1;
	if (opt_display.readBin(f) < 0) return -1;
	if (opt_drawing.readBin(f) < 0) return -1;
	
	assert(opt_drawing_creator != NULL);
	return opt_drawing_creator->readBin(f);
}

int OptAppDrawing::saveBin(FILE* f) const {
	if (opt_camera.saveBin(f) < 0) return -1;
	if (opt_camera_pos.saveBin(f) < 0) return -1;
	if (opt_display.saveBin(f) < 0) return -1;
	if (opt_drawing.saveBin(f) < 0) return -1;
	
	assert(opt_drawing_creator != NULL);
	return opt_drawing_creator->saveBin(f);
}

