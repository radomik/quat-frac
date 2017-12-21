// QuatFracDrawing.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "FractalWindowLoader.hpp"
#include "utils.hpp"

#ifdef _WINDOWS
	#ifdef UNICODE
		int _tmain(int argc, wchar_t** argv)
	#else
		int _tmain(int argc, char** argv)
	#endif
#else
	int main(int argc, char** argv)
#endif
{
	// obtain instance of FractalWindowLoader
	FractalWindowLoader* loader = FractalWindowLoader::get();
	
	// ProgramArgs allocates some memory - they must be deleted before loader.start()
	// function: initialize() returns only on success
	loader->initialize(argc, argv);
	
	loader->start();
	
	return 0;
}

