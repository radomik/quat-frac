#include "stdafx.h"
#include "DrawingCreator.hpp"
#include "OptGenerator.hpp"
#include "OptDrawingCreator.hpp"
#include "BinSerialization.hpp"
#include "GLdrawing.hpp"
#include "timing.h"

int DrawingCreator::_st_checkVersions(
		unsigned int				opt_generator_version, 
		const OptDrawingCreator&	opt_drawing)
{
	if (opt_generator_version != opt_drawing.version) {
		fprintf(stderr, "[ERROR] opt_generator.version: %u and opt_drawing.version: %u mismatch\n\n",
			opt_generator_version, opt_drawing.version);
		return -1;
	}
	
	return 0;
}

int DrawingCreator::create(
		FILE*						fractal_data_file, 
		const OptDrawingCreator&	opt_drawing)
{
	reset_and_start_timer();
	
	const char *self_name = "DrawingCreator::create";
	const char *self_real_name = getName();
	unsigned int opt_generator_version = getVersion();
	unsigned int count_data_points;

	fprintf(stderr, "Starting drawing creator:    %s\n", self_real_name);
	fprintf(stderr, "  * opt_generator_version:   %u\n", opt_generator_version);
	
	// compare opt_generator and opt_drawing versions
	if (_st_checkVersions(opt_generator_version, opt_drawing) < 0)
		return -1;
	
	// check if opt_drawing has valid type
	if (checkOpt(opt_drawing) < 0) return -1;
	
	// Read count of data points
	if (BinSerialization::readValue<unsigned int>(fractal_data_file, count_data_points) < 0) {
		SysError ex;
		fprintf(stderr, "%s: Failed to read count of data points [%s]\n", self_name, ex.msg);
		return -1;
	}
	fprintf(stderr, "  * count_data_points:       %u\n", count_data_points);
	
	// read all file into array 
	// (there is a huge performance overhead while reading directly from file on Windows,
	//	I missing something ? It's easier and faster to read whole file to memory temporary,
	//	it's at all the smallest buffer from the other)

	unsigned char	*file_data = _readFileIntoArray(fractal_data_file, count_data_points);
	if (!file_data) {
		fprintf(stderr, "%s Failed to read file into array\n", self_name);
		return -1;
	}

	// fill display lists
	if (createCustom(m_drawing, file_data, opt_drawing, count_data_points) < 0) {
		fprintf(stderr, "%s: createCustom() failed\n", self_name);
		return -1;
	}

	// delete file data buffer
	//fprintf(stderr, "[DBG] %s @ %d [DEL] %p (unsigned char[] file_data)\n", __FILE__, __LINE__, file_data);
	delete[] file_data;
	
	// select active display list
	m_drawing.toggleDisplist(opt_drawing.disp_list_ind);
	
	fprintf(stderr, "Created display objects in %.3f mcycles\n", get_elapsed_mcycles());

	return 0;
}

unsigned char* DrawingCreator::_readFileIntoArray(
	FILE			*fractal_data_file,
	unsigned int	count_data_points)
{
	//reset_and_start_timer();

	// Size in bytes of data from file
	size_t data_size = count_data_points * getDrawingElementByteSize();

	char strSize[64];
	formatBytes(data_size, strSize, sizeof(strSize));
	fprintf(stderr, "Allocating %s for bin file data\n", strSize);

	// Allocate array for data from file
	unsigned char * file_data;
	try {
		file_data = new unsigned char[data_size];
		//fprintf(stderr, "[DBG] %s @ %d [NEW] %p (unsigned char[] file_data)\n", __FILE__, __LINE__, file_data);
	}
	catch (const std::bad_alloc& e) {
		SysError ex;
		fprintf(stderr, "Failed to allocate %s for file data [what: %s, errno: %s]\n", strSize, e.what(), ex.msg);
		return NULL;
	}

	// Read data from file into array
	//fprintf(stderr, "Reading data from file\n");
	if (fread(file_data, sizeof(unsigned char), data_size, fractal_data_file) != data_size)
	{
		SysError ex;
		fprintf(stderr, "Failed to read %s bytes of data from file [%s]\n", strSize, ex.msg);
		//fprintf(stderr, "[DBG] %s @ %d [DEL] %p (unsigned char[] file_data)\n", __FILE__, __LINE__, file_data);
		delete[] file_data;
		return NULL;
	}

	//fprintf(stderr, "Read data from file into array in %.3f mcycles\n", get_elapsed_mcycles());
	return file_data;
}
