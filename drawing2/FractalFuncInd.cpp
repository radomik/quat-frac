#include "stdafx.h"
#include "Integer.hpp"
#include "FractalFuncInd.hpp"
#include "BinSerialization.hpp"

void FractalFuncInd::parse(const std::string &str)
{
  unsigned int value_uint;

  try {
    value_uint = U32::parse2(str);
  } catch (const std::exception &e) {
    Q_THROW(this, "Not a valid uint: '%s':\n%s", str.c_str(), e.what());
  }
  if ((value_uint == 0) || (value_uint > FractalFuncCount)) {
    Q_THROW(this, "Value: %u exceeds fractal_func_ind_t range (expected value in range: [1 ... %d])",
      value_uint, FractalFuncCount);
  }
  this->value = (fractal_func_ind_t)(value_uint - 1);
}

void FractalFuncInd::parse(int value_int)
{
  if ((value_int <= 0) || (value_int > FractalFuncCount)) {
    Q_THROW(this, "Value: %d exceeds fractal_func_ind_t range (expected value in range: [1 ... %d])",
      value_int, FractalFuncCount);
  }
  this->value = (fractal_func_ind_t)(value_int - 1);
}

std::string FractalFuncInd::toString() const
{
  return std::to_string((unsigned int)(this->value + 1));
}

void FractalFuncInd::readBin(FILE *file, const char *tag)
{
  BinSerialization::readValueAsOneByte(f, this->value, tag);
}

void FractalFuncInd::saveBin(FILE *file, const char *tag) const
{
  BinSerialization::writeValueAsOneByte(f, this->value, tag);
}
