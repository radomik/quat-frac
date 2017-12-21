/*
* File:   Generator.hpp
* Author: darek
*
* Created on 11 December 2014, 12:25
*/
#ifndef QFRACTAL_GENERATOR_GENERATOR_HPP
#define QFRACTAL_GENERATOR_GENERATOR_HPP

#include "utils.hpp"
#include "FractalFunc.hpp"

class OptGenerator;

class Generator {
public:
  virtual ~Generator() { }

  int compute(FILE *out_file, FILE *log_file = NULL);

  static void printCountCreated(size_t count) {
#ifdef _WINDOWS
    fprintf(stderr, "\rCreated %9Iu data points", count);
#else
    fprintf(stderr, "\rCreated %9zu data points", count);
#endif
  }

protected:

  virtual int computeCustom(FILE *out_file) = 0;
  virtual const char*  getName() const = 0;
  virtual unsigned int getDataItemSize() const = 0;

  virtual unsigned int getVersion() const = 0;

  // available after computeCustom()
  virtual size_t getCountInPoints() const { return 0; }
  virtual size_t getTotalRecursIterCount() const { return 0; }

  virtual const OptGenerator& getOptions() const = 0;

  fractal_func_ind_t getFuncIndex() const;

private:
  void _printSummary(long int pos2, unsigned int count, FILE *log_file = NULL);
};

#endif
