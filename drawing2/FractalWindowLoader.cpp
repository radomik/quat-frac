#include "stdafx.h"
#include "FractalWindowLoader.hpp"
#include "Parser.hpp"
#include "ProgramArgs.hpp"
#include "OptAppDrawingParser.hpp"
#include "Name.hpp"
#include "lodepng.hpp"
#include <ctime>

FractalWindowLoader FractalWindowLoader::INSTANCE;

int FractalWindowLoader::init() {
	m_file_save_options.resize(0);
	return m_fractal_window.init();
}

#ifdef UNICODE
const wchar_t* FractalWindowLoader::OP_IN_BIN = L"-i";
const wchar_t* FractalWindowLoader::OP_IN_CONF = L"-c";
const wchar_t* FractalWindowLoader::OP_OUT_CONF = L"-s";
const wchar_t* FractalWindowLoader::OP_OUT_SCR = L"-p";

void FractalWindowLoader::_st_printUsage(int argc, wchar_t** argv) {
	_tprintf(L"Usage: %s %s <input bin file> \
[ %s <input conf file> ] \
[ %s <output save conf file> ] \
[ %s <output screenshot directory>]\n\
\n\
Program created as a part of thesis in Bialystok Technical University \
department of Computer Science.\n\n",
		argv[0], OP_IN_BIN, OP_IN_CONF, OP_OUT_CONF, OP_OUT_SCR);
}
#else
const char* FractalWindowLoader::OP_IN_BIN = "-i";
const char* FractalWindowLoader::OP_IN_CONF = "-c";
const char* FractalWindowLoader::OP_OUT_CONF = "-s";
const char* FractalWindowLoader::OP_OUT_SCR = "-p";

void FractalWindowLoader::_st_printUsage(int argc, char** argv) {
	printf("Usage: %s %s <input bin file> \
[ %s <input conf file> ] \
[ %s <output save conf file> ] \
[%s <output screenshot directory>]\n\
\n\
Program created as a part of thesis in Bialystok Technical University \
department of Computer Science.\n\n",
		argv[0], OP_IN_BIN, OP_IN_CONF, OP_OUT_CONF, OP_OUT_SCR);
}
#endif

#ifdef UNICODE
void FractalWindowLoader::initialize(int argc, wchar_t** argv) {
	// initialize program arguments
	ProgramArgs args(argc, argv);

	if (args.init() < 0) {
		fprintf(stderr, "Failed to initialize program arguments\n");
		exit(-1);
	}

	// check if has help option
	if ((argc == 1) || (args.hasHelpOption())) {
		_st_printUsage(argc, argv);
		exit(0);
	}

	// initialize loader
	if (init() < 0) {
		fprintf(stderr, "Failed to initialize FractalWindowLoader\n");
		exit(-1);
	}

	// get fractal file path (required)
	std::wstring bin_file_path;
	if (!args.getOption(bin_file_path, OP_IN_BIN)) {
		_st_printUsage(argc, argv);
		printf("\nMissing argument: fractal bin file path\n");
		exit(0);
	}

	// get options file path (optional)
	std::wstring conf_file_path;
	args.getOption(conf_file_path, OP_IN_CONF);

	// get save options file path optional
	args.getOption(m_file_save_options, OP_OUT_CONF);

	// get png output directory
	args.getOption(m_png_out_dir, OP_OUT_SCR);

	// create name
	m_name = getFilename(bin_file_path.c_str()); // extract filename from bin filepath
	replaceOrAppendExt(m_name, L""); // remove extension

	// load fractal and settings
	int ret;
	if ((ret = _load(args, bin_file_path, conf_file_path)) < 0) {
		fprintf(stderr, "Failed to load fractal\n");
		exit(-ret);
	}
}
#else
void FractalWindowLoader::initialize(int argc, char** argv) {
	// initialize program arguments
	ProgramArgs args(argc, argv);

	if (args.init() < 0) {
		fprintf(stderr, "Failed to initialize program arguments\n");
		exit(-1);
	}

	// check if has help option
	if ((argc == 1) || (args.hasHelpOption())) {
		_st_printUsage(argc, argv);
		exit(0);
	}

	// initialize loader
	if (init() < 0) {
		fprintf(stderr, "Failed to initialize FractalWindowLoader\n");
		exit(-1);
	}

	// get fractal file path (required)
	std::string bin_file_path;
	if (!args.getOption(bin_file_path, OP_IN_BIN)) {
		_st_printUsage(argc, argv);
		printf("\nMissing argument: fractal bin file path\n");
		exit(0);
	}

	// get options file path (optional)
	std::string conf_file_path;
	args.getOption(conf_file_path, OP_IN_CONF);

	// get save options file path optional
	args.getOption(m_file_save_options, OP_OUT_CONF);

	// get png output directory
	args.getOption(m_png_out_dir, OP_OUT_SCR);

	// create name
	m_name = getFilename(bin_file_path.c_str()); // extract filename from bin filepath
	replaceOrAppendExt(m_name, ""); // remove extension

	// load fractal and settings
	int ret;
	if ((ret = _load(args, bin_file_path, conf_file_path)) < 0) {
		fprintf(stderr, "Failed to load fractal\n");
		exit(-ret);
	}
}
#endif

FractalWindowLoader::~FractalWindowLoader() {
	if (m_opt_generator) {
		//fprintf(stderr, "[DBG] %s @ %d [DEL] %p (OptGenerator)\n", __FILE__, __LINE__, m_opt_generator);
		delete(m_opt_generator);
		m_opt_generator = NULL;
	}
}

#ifdef UNICODE
int FractalWindowLoader::_load(ProgramArgs& args, const std::wstring &bin_file_path, const std::wstring &conf_file_path)
#else
int FractalWindowLoader::_load(ProgramArgs& args, const std::string &bin_file_path, const std::string &conf_file_path)
#endif
{
	if (bin_file_path.empty()) {
		fprintf(stderr, "Fractal bin file path is empty\n");
		return -1;
	}

	// open fractal data file
	FILE     *fd_fractal;
#ifdef _WINDOWS
#ifdef UNICODE
	if (_wfopen_s(&fd_fractal, bin_file_path.c_str(), L"rb"))
#else
	if (fopen_s(&fd_fractal, bin_file_path.c_str(), "rb"))
#endif
#else
	if (!(fd_fractal = fopen(bin_file_path.c_str(), "rb")))
#endif
	{
		SysError ex;
#ifdef UNICODE
		fwprintf(stderr, L"Failed to open fractal binary file: '%s' ", bin_file_path.c_str());
#else
		fprintf(stderr, "Failed to open fractal binary file: '%s' ", bin_file_path.c_str());
#endif
		fprintf(stderr, "[%s]\n", ex.msg);
		return -1;
	}

	// load settings and fractal
	int ret = _loadState(fd_fractal, args, conf_file_path);

	// close fractal data file
	fclose(fd_fractal);

	return ret;
}

#ifdef UNICODE
int FractalWindowLoader::_loadState(FILE* fd_fractal, ProgramArgs& args, const std::wstring &conf_file_path)
#else
int FractalWindowLoader::_loadState(FILE* fd_fractal, ProgramArgs& args, const std::string &conf_file_path)
#endif
{	
	// read generator options (version may be needed to create default OptAppDrawing
	if (_readOptGenerator(fd_fractal) < 0) return -1;
	
	return _initSettingsAndFractal(fd_fractal, args, conf_file_path);
}

#ifdef UNICODE
int FractalWindowLoader::_initSettingsAndFractal(FILE* fd_fractal, ProgramArgs& args, const std::wstring &conf_file_path)
#else
int FractalWindowLoader::_initSettingsAndFractal(FILE* fd_fractal, ProgramArgs& args, const std::string &conf_file_path)
#endif
{
	OptAppDrawingParser settings;
	
	// read settings from given file or create default settings
	assert(m_opt_generator);
	if (settings.readOrDefault(conf_file_path, m_opt_generator->version) < 0) {
		fprintf(stderr, "Failed to read opt_app_drawing or load default\n");
		return -1;
	}
	
	// just display current settings on stderr using Parser (remove later)
	/*{
		fprintf(stderr, "%s: Obtained settings:\n", __FUNCTION__);
		Parser dbg_show_settings(stderr, PARSER_MODE_SAVE);
		settings.saveTextCurrent(dbg_show_settings);
		fprintf(stderr, "\n");
	}*/
	
	// initialize graphics with given settings (before creating fractal drawing)
	m_fractal_window.initGfx(settings, args);
	
	// create fractal drawing
	int ret;
	if ((ret=m_fractal_window.createDrawing(fd_fractal, *m_opt_generator, *settings.opt_app_drawing.opt_drawing_creator)) < 0) {
		fprintf(stderr, "Failed to create fractal drawing\n");
		return ret;
	}
	
	return 0;
}

int FractalWindowLoader::_readOptGenerator(FILE* fd_fractal) 
{
	if (m_opt_generator) {
		//fprintf(stderr, "[DBG] %s @ %d [DEL] %p (OptGenerator)\n", __FILE__, __LINE__, m_opt_generator);
		delete(m_opt_generator);
	}
	
	// read generator param from fractal data file
	m_opt_generator = OptGenerator::readNewByBin(fd_fractal);
	if (! m_opt_generator) {
		fprintf(stderr, "Failed read generator parameters\n");
		return -1;
	}
	
	return 0;
}

int FractalWindowLoader::save(const OptAppDrawing& opt_app_drawing) const 
{
	if (m_file_save_options.empty()) {
		fprintf(stderr, "Cannot save: m_file_save_options is empty\n");
		return -1;
	}
	
	FILE *fd_opt = _openFileForSave();
	if (!fd_opt) return -1;

	int ret = _st_save(fd_opt, opt_app_drawing, m_opt_generator);

	if (ret == 0) {
#ifdef UNICODE
		fwprintf(stderr, L"\nSaved config file '%s'\n", m_file_save_options.c_str());
#else
		fprintf(stderr, "\nSaved config file '%s'\n", m_file_save_options.c_str());
#endif
	}

	fclose(fd_opt);
	return ret;
}

int FractalWindowLoader::exportPng(std::vector<unsigned char> &pixel_data, unsigned int w, unsigned h) {
	// Encode pixel data to PNG
	std::vector<unsigned char>  png_image;
	unsigned 					error;
	//fprintf(stderr, "%s: Encoding pixel data\n", __FUNCTION__);
	if ((error = lodepng::encode(png_image, pixel_data, w, h)) != 0) {
		std::cerr << __FUNCTION__ << ": Encoder error " << error << ": " << lodepng_error_text(error) << std::endl;
		return -1;
	}

	const char *time_fmt = "_%Y%m%d%H%M%S";
	char time_str[16];
	time_t rt;
	time(&rt);

#ifdef _WINDOWS
	struct tm tinfo;
	if (localtime_s(&tinfo, &rt) != 0) {
		time_str[0] = '\0';
	}
	else strftime(time_str, sizeof(time_str), time_fmt, &tinfo);
#else
	struct tm *tinfo;
	tinfo = localtime(&rt);
	strftime(time_str, sizeof(time_str), time_fmt, tinfo);
#endif

	//printf("time_str: '%s'\n", time_str);

	// Create output path
#ifdef UNICODE
	std::wstring filename = m_png_out_dir;
	wchar_t      ind_str[16];
	wchar_t      w_time_str[sizeof(time_str)];
	unsigned int i;
	for (i = 0; i < sizeof(time_str) && time_str[i]; i++) {
		w_time_str[i] = (wchar_t)time_str[i];
	}
	w_time_str[i] = L'\0';

	//fwprintf(stderr, L"w_time_str: '%s'\n", w_time_str);

	if (! filename.empty()) filename.append(W_FILEPATH_SEPARATOR_STR);
	filename.append(m_name);
	sys_snwprintf(ind_str, sizeof(ind_str) / sizeof(ind_str[0]), L"_%02u", m_png_index);
	m_png_index++;
	filename.append(ind_str);
	filename.append(w_time_str);
	filename.append(L".png");
#else
	std::string filename = m_png_out_dir;
	char      ind_str[16];
	if (! filename.empty()) filename.append(FILEPATH_SEPARATOR_STR);
	filename.append(m_name);
	sys_snprintf(ind_str, sizeof(ind_str) / sizeof(ind_str[0]), "_%02u", m_png_index);
	m_png_index++;
	filename.append(ind_str);
	filename.append(time_str);
	filename.append(".png");
#endif

	// Save PNG file
	lodepng::save_file(png_image, filename);
	
#ifdef UNICODE
	fwprintf(stderr, L"Saved image to file: '%s'\n", filename.c_str());
#else
	fprintf(stderr, "Saved image to file: '%s'\n", filename.c_str());
#endif
	
	return 0;
}

int FractalWindowLoader::_st_save(
		FILE*					fd_opt,
		const OptAppDrawing&	opt_app_drawing,
		const OptGenerator*		opt_generator)
{
	Parser writer(fd_opt, PARSER_MODE_SAVE);

	OptAppDrawingParser opt_app_drawing_parser;
	
	if (opt_app_drawing_parser.saveText(writer, opt_app_drawing) < 0) {
		fprintf(stderr, "Cannot save: opt_app_drawing_parser.saveText() failed\n");
		return -1;
	}
	
	if (opt_generator) {
		if (writer.writeObject(*opt_generator, Name::opt_generator) < 0) {
			fprintf(stderr, "Cannot save: opt_generator\n");
			return -1;
		}
	}
	
	return 0;
}

FILE* FractalWindowLoader::_openFileForSave() const {
	FILE *fd;
#ifdef _WINDOWS
#ifdef UNICODE
	if (_wfopen_s(&fd, m_file_save_options.c_str(), L"w")) {
		SysError ex;
		fwprintf(stderr, 
			L"Cannot save: failed to open file: '%s' for writing [%s]\n", m_file_save_options.c_str(), ex.msg);
		return NULL;
	}
#else
	if (fopen_s(&fd, m_file_save_options.c_str(), "w")) {
		SysError ex;
		fprintf(stderr,
			"Cannot save: failed to open file: '%s' for writing\n", m_file_save_options.c_str(), ex.msg);
		return NULL;
	}
#endif
#else
	if (!(fd = fopen(m_file_save_options.c_str(), "w"))) {
		SysError ex;
		fprintf(stderr,
			"Cannot save: failed to open file: '%s' for writing [%s]\n", m_file_save_options.c_str(), ex.msg);
		return NULL;
	}
#endif
	return fd;
}
