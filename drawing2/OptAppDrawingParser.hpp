/*
 * File:   OptAppDrawingParser.hpp
 * Author: darek
 *
 * Created on 23 November 2014, 23:35
 */
#ifndef QFRACTAL_DRAWING_OPTAPPDRAWINGPARSER_HPP
#define QFRACTAL_DRAWING_OPTAPPDRAWINGPARSER_HPP

#include "utils.hpp"
#include "ISerializable.hpp"
#include "OptAppDrawing.hpp"

/** Class representing settings read/written to fractal drawing program
 * configuration file */
class OptAppDrawingParser : protected ISerializable {
public:
	OptAppDrawing			opt_app_drawing;

	int	setDefault(unsigned int generator_version);

	int saveText(Parser& parser, const OptAppDrawing& opt_app_drawing) {
		_setSaveOpt(opt_app_drawing);
		return saveText(parser);
	}

	int saveBin(FILE* f, const OptAppDrawing& opt_app_drawing) {
		_setSaveOpt(opt_app_drawing);
		return saveBin(f);
	}

	int saveTextCurrent(Parser& parser) { return saveText(parser, opt_app_drawing); }
	int saveBinCurrent(FILE* f)         { return saveBin(f, opt_app_drawing); }

#ifdef UNICODE
	int readOrDefault(const std::wstring& settings_file, unsigned int generator_version);
#else
	int readOrDefault(const std::string& settings_file, unsigned int generator_version);
#endif

protected:
	virtual int readText(Parser& parser);
	virtual int saveText(Parser& parser) const; // (private)

	virtual void readBin(FILE* f);
	virtual void saveBin(FILE* f) const;

private:
	const OptAppDrawing*	m_opt_app_drawing_save;

	void _setSaveOpt(const OptAppDrawing& opt) { m_opt_app_drawing_save = &opt; }
};

#endif
