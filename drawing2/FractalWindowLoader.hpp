/*
 * File:   FractalWindowLoader.hpp
 * Author: darek
 *
 * Created on 23 November 2014, 23:05
 */
#ifndef QFRACTAL_DRAWING_FRACTALWINDOWLOADER_HPP
#define QFRACTAL_DRAWING_FRACTALWINDOWLOADER_HPP

#include "FractalWindow.hpp"
#include "OptAppDrawing.hpp"
#include "OptGenerator.hpp"
#include "utils.hpp"

class FractalWindowLoader {
public:
	/// get singleton instance
	static FractalWindowLoader* get() { return &INSTANCE; }
	
	int init();
	
	void start() {
		m_fractal_window.start();
	}

	int save(const OptAppDrawing& opt_app_drawing) const;
	int exportPng(std::vector<unsigned char> &pixel_data, unsigned int w, unsigned h);
	
	#ifdef UNICODE
		void initialize(int argc, wchar_t** argv);
		void getName(std::wstring &out_path) { out_path = m_name; }
	#else
		void initialize(int argc, char** argv);
		void getName(std::string &out_path) { out_path = m_name; }
	#endif
	
	FractalWindowLoader() {
		m_opt_generator = NULL;
		m_png_index = 1;
	}
	
	~FractalWindowLoader();
private:
	int   _readOptGenerator(FILE* fd_fractal);
	int   _initOptAppDrawingParser();
	FILE* _openFileForSave() const;

	static int _st_save(
		FILE*					fd_opt,
		const OptAppDrawing&	opt_app_drawing,
		const OptGenerator*		opt_generator);
						
#ifdef UNICODE
	void  _st_printUsage(int argc, wchar_t **argv);
	int   _load(ProgramArgs& args, const std::wstring &bin_file_path, const std::wstring &conf_file_path);
	int   _loadState(FILE* fd_fractal, ProgramArgs& args, const std::wstring &conf_file_path);
	int   _initSettingsAndFractal(FILE* fd_fractal, ProgramArgs& args, const std::wstring &conf_file_path);
#else
	void  _st_printUsage(int argc, char **argv);
	int   _load(ProgramArgs& args, const std::string &bin_file_path, const std::string &conf_file_path);
	int   _loadState(FILE* fd_fractal, ProgramArgs& args, const std::string &conf_file_path);
	int   _initSettingsAndFractal(FILE* fd_fractal, ProgramArgs& args, const std::string &conf_file_path);
#endif

	static FractalWindowLoader INSTANCE;
	
	FractalWindow	 m_fractal_window;
	OptGenerator	 *m_opt_generator;
	unsigned int     m_png_index;

#ifdef UNICODE
	static const wchar_t* OP_IN_BIN;
	static const wchar_t* OP_IN_CONF;
	static const wchar_t* OP_OUT_CONF;
	static const wchar_t* OP_OUT_SCR;

	std::wstring		 m_png_out_dir;       // output directory path for PNG files
	std::wstring		 m_file_save_options; // output filepath for options
	std::wstring		 m_name;			  // name of binary file without path and extension 
#else
	static const char* OP_IN_BIN;
	static const char* OP_IN_CONF;
	static const char* OP_OUT_CONF;
	static const char* OP_OUT_SCR;

	std::string		 	 m_png_out_dir;       // output directory path for PNG files
	std::string			 m_file_save_options; // output filepath for options
	std::string			 m_name;			  // name of binary file without path and extension 
#endif
};

#endif

