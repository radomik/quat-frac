/*
* File:   FractalGen.hpp
* Author: darek
*
* Created on 22 November 2014, 16:46
*/
#ifndef QFRACTAL_GENERATOR_FRACTALGEN_HPP
#define QFRACTAL_GENERATOR_FRACTALGEN_HPP

#include "utils.hpp"
#include "OptGenerator.hpp"

class FractalGen {
public:

#ifdef _UTF16
  FractalGen(const wchar_t *out_filename);
#else
  FractalGen(const char *out_filename);
#endif

  int generate(const OptGenerator &opt, FILE *log_file = NULL);

  bool&                 saveParam()                  { return m_save_param; }
  const bool&           saveParamConst() const       { return m_save_param; }

private:
  int  _openFile();
  int  _generateAndSave(const OptGenerator &opt, FILE *log_file = NULL);
  int  _saveParam(const OptGenerator &opt);
  int  _computeFractal(const OptGenerator &opt, FILE *log_file = NULL);
  void _cleanUp();

#ifdef _UTF16
  const wchar_t *m_filename;
#else
  const char    *m_filename;
#endif

  FILE      *m_file;
  bool      m_save_param;
};


#endif


