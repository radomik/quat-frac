#include "stdafx.h"
#include "FractalGen.hpp"
#include "Generator.hpp"
#include "FractalFunc.hpp"
#include "BinSerialization.hpp"

#ifdef _UTF16
FractalGen::FractalGen(const wchar_t *out_filename)
#else
FractalGen::FractalGen(const char *out_filename)
#endif
{
  m_filename = out_filename;
  m_file = NULL;
  m_save_param = true;
}

int FractalGen::generate(const OptGenerator &opt, FILE *log_file) {
  int ret;

  // open destination file
  if (_openFile() != 0) return -1;

  // generate and save fractal data to opened file
  ret = _generateAndSave(opt, log_file);

  // close destination file
  _cleanUp();

  return ret;
}

int FractalGen::_generateAndSave(const OptGenerator &opt, FILE *log_file) {
  // save generator parameters if needed
  if (_saveParam(opt) != 0) return -1;

  // compute and save fractal
  return _computeFractal(opt, log_file);
}

int FractalGen::_openFile() {
#ifdef _UTF16
  errno_t err = _wfopen_s(&m_file, m_filename, L"wb");
  if (err) {
    SysError ex;
    fwprintf(stderr, L"FractalGen: Failed to open input file: '%s' ", m_filename);
    fprintf(stderr, "[%s]\n", ex.msg);
    return -1;
  }
#else
  m_file = fopen(m_filename, "wb");
  if (!m_file) {
    SysError ex;
    fprintf(stderr, "FractalGen: Failed to open input file: '%s' [%s]\n", m_filename, ex.msg);
    return -1;
  }
#endif
  return 0;
}

int FractalGen::_saveParam(const OptGenerator &opt) {
  if (!m_save_param) return 0;

  if (opt.saveBin(m_file) != 0) {
    fprintf(stderr, "FractalGen: Failed to write fractal parameters to file\n");
    return -1;
  }

  return 0;
}

int FractalGen::_computeFractal(const OptGenerator &opt, FILE *log_file) {
  Generator *gen = opt.getNewGenerator();

  if (!gen) {
    fprintf(stderr, "FractalGen::_computeFractal() Failed generator is NULL\n");
    return -1;
  }

  int ret = gen->compute(m_file, log_file);
  delete gen;
  return ret;
}

void FractalGen::_cleanUp() {
  if (m_file) {
    fclose(m_file);
    m_file = NULL;
  }
}
