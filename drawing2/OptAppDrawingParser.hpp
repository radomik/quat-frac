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
  OptAppDrawing     opt_app_drawing;

  int setDefault(unsigned int generator_version);

  nlohmann::json saveJson(const char *tag, const OptAppDrawing& opt_app_drawing) {
    _setSaveOpt(opt_app_drawing);
    return saveJson(tag);
  }

  void saveBin(FILE *file, const char *tag, const OptAppDrawing& opt_app_drawing) {
    _setSaveOpt(opt_app_drawing);
    saveBin(file, tag);
  }

  nlohmann::json saveJsonCurrent(const char *tag) { return saveJson(tag, opt_app_drawing); }
  void saveBinCurrent(FILE *file, const char *tag) { return saveBin(file, tag, opt_app_drawing); }

#ifdef UNICODE
  int readOrDefault(const std::wstring& settings_file, unsigned int generator_version);
#else
  int readOrDefault(const std::string& settings_file, unsigned int generator_version);
#endif

protected:
  virtual void readJson(const nlohmann::json &json, const char *tag);
  virtual nlohmann::json saveJson(const char *tag) const;

  virtual void readBin(FILE *file, const char *tag);
  virtual void saveBin(FILE *file, const char *tag) const;

  virtual void validate(const char *tag) const;

private:
  const OptAppDrawing*  m_opt_app_drawing_save;

  void _setSaveOpt(const OptAppDrawing& opt) { m_opt_app_drawing_save = &opt; }
};

#endif
