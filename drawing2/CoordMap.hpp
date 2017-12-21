/*
* File:   CoordMap.hpp
* Author: darek
*
* Created on 10 December 2014, 1:16
*/
#ifndef QFRACTAL_DRAWING_COORDMAP_H
#define QFRACTAL_DRAWING_COORDMAP_H

#include "IPrimitive.hpp"
#include "Quat.hpp"
#include "utils.hpp"

class CoordMap : public IPrimitive {
public:
  static const uint8_t SIZE;
  static const char *VALUES;
  unsigned char value[3];

  QuatInd  operator [](unsigned int i) const { return (QuatInd)value[i]; }
  QuatInd& operator [](unsigned int i)       { return (QuatInd)value[i]; }

  virtual void parse(const std::string &str);
  virtual std::string toString() const;
  virtual void readBin(FILE *file, const char *tag);
  virtual void saveBin(FILE *file, const char *tag) const;

private:
  static QuatInd _st_charToInd(char c);
};

#endif


