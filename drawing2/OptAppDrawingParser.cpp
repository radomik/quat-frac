#include "stdafx.h"
#include "OptAppDrawingParser.hpp"
#include "Parser.hpp"
#include "Name.hpp"

int OptAppDrawingParser::setDefault(unsigned int generator_version) {
  return opt_app_drawing.setDefault(generator_version);
}

#ifdef UNICODE
int OptAppDrawingParser::readOrDefault(
  const std::wstring& settings_file,
  unsigned int    generator_version)
#else
int OptAppDrawingParser::readOrDefault(
  const std::string&  settings_file,
  unsigned int    generator_version)
#endif
{
  if (! settings_file.empty())
  {
    FILE *file;
#ifdef _WINDOWS
#ifdef UNICODE
    if (_wfopen_s(&file, settings_file.c_str(), L"rb"))
#else
    if (fopen_s(&file, settings_file.c_str(), "rb"))
#endif
#else
    if (! (file = fopen(settings_file.c_str(), "rb")))
#endif
    {
      SysError ex;
      fprintf(stderr, "%s Cannot open options file [%s]. Loading default settings\n", __FUNCTION__, ex.msg);
      if (setDefault(generator_version) < 0) return -1;
      return 0;
    }
    Parser parser(file, PARSER_MODE_READ);

    if (readText(parser) < 0) {
      fprintf(stderr, "Warning: Failed to load settings file. Loading default settings\n");
      fprintf(stderr, "[INFO] Parser stopped on line:\n");
      fprintf(stderr, "%u: %s\n", parser.getPrevLineNo(), parser.getLine());
      if (setDefault(generator_version) < 0) return -1;
      return 0;
    }
    fclose(file);
  }
  else { // m_file_settings is NULL
    //fprintf(stderr, "%s [DBG] settings_file is \"\". Loading default settings\n", __FUNCTION__);
    if (setDefault(generator_version) < 0) return -1;
    return 0;
  }

  return 0;
}

void OptAppDrawingParser::readJson(const nlohmann::json &json, const char *tag) {
  const char* self_name = "OptAppDrawingParser";

  bool rd_opt_app_drawing = false;

  int ret;

  while (! parser.isEof()) {
    if (parser.parseLine() < 0)
      return -1;

    if (parser.isClosureEnd()) break;

    if (parser.isClosureBegin()) {
      if ((ret=parser.parseClosure(opt_app_drawing, Name::opt_app_drawing, &rd_opt_app_drawing, self_name)) < 0) return -1;
      if ((ret > 0) && (rd_opt_app_drawing)) continue;

      if (parser.omitClosure() < 0) return -1;
    }
    else {
      // skip all tokens
      //if (parser.isToken()) {
        //parser.unexpectedToken(self_name);
        //return -1;
      //}
    }
  }

  if ((parser.isFull()) && (! (rd_opt_app_drawing)))
  {
    fprintf(stderr, "%s: Partially parsed:\n\
Have{opt_app_drawing}={%d}\n", self_name, rd_opt_app_drawing);
    return -1;
  }

  return 0;
}

nlohmann::json OptAppDrawingParser::saveJson(const char *tag) const {
  assert(m_opt_app_drawing_save != NULL);
  return parser.writeObject(*m_opt_app_drawing_save, Name::opt_app_drawing);
}

void OptAppDrawingParser::readBin(FILE *file, const char *tag) {
  opt_app_drawing.readBin(file, tag);
}

void OptAppDrawingParser::saveBin(FILE *file, const char *tag) const {
  assert(m_opt_app_drawing_save != NULL);
  return m_opt_app_drawing_save->saveBin(f);
}
