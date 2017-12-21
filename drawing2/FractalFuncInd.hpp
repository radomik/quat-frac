/*
* File:   FractalFuncInd.hpp
* Author: darek
*
* Created on 24 November 2014, 00:24
*/
#ifndef QFRACTAL_FRACTALFUNCIND_HPP
#define QFRACTAL_FRACTALFUNCIND_HPP

#include "IPrimitive.hpp"
#include "utils.hpp"

enum fractal_func_ind_t {
  FractalFuncPow1 = 0,
  FractalFuncPow2,
  FractalFuncPow3,
  FractalFuncPow4,
  FractalFuncPow5,
  FractalFuncPow6,
  FractalFuncSin,
  FractalFuncCos,
  FractalFuncCount
};

class FractalFuncInd : public IPrimitive {
public:
  fractal_func_ind_t value;

  virtual void parse(const std::string &str);
  virtual std::string toString() const;

  virtual void readBin(FILE *file, const char *tag);
  virtual void saveBin(FILE *file, const char *tag) const;

  void parse(int value_int);
};

#endif
