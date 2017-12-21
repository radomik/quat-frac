/*
* File:   IPrimitive.hpp
* Author: darek
*
* Created on 20 November 2014, 12:40
*/
#ifndef QFRACTAL_IPRIMITIVE_HPP
#define QFRACTAL_IPRIMITIVE_HPP

#include "utils.hpp"

class IPrimitive {
public:
  virtual void parse(const std::string &str) = 0;
  virtual std::string toString() const = 0;

  virtual void readBin(FILE *file, const char *tag) = 0;
  virtual void saveBin(FILE *file, const char *tag) const = 0;
};

#endif


