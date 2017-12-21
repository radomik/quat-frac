/*
* File:   FloatingPoint.h
* Author: darek
*
* Created on 20 November 2014, 12:40
*/
#ifndef QFRACTAL_FLOATINGPOINT_H
#define QFRACTAL_FLOATINGPOINT_H

#include "IPrimitive.hpp"
#include "utils.hpp"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace FloatingPoint {
  template<typename T>
  int cmp(T threshold, T a, T b);
};

class Float : public IPrimitive {
public:
  float value;

  virtual void parse(const std::string &str);
  virtual std::string toString() const;
  virtual void readBin(FILE *file, const char *tag);
  virtual void saveBin(FILE *file, const char *tag) const;

  bool operator== (const Float &other) const {
    return equ(value, other.value);
  }

  int cmp(const Float &other) const {
    return cmp(value, other.value);
  }

  int cmp(float value) const {
    return cmp(this->value, value);
  }

  bool checkAboveZero(const char *name) const { return checkAboveZero(value, name); }

  static bool equ(float a, float b) {
    return fabsf(a - b) < FLT_EPSILON;
  }

  static int cmp(float a, float b);
  static bool checkAboveZero(float value, const char *name);
  static float parse2(const std::string &str);
};


class Double : public IPrimitive {
public:
  double value;

  virtual void parse(const std::string &str);
  virtual std::string toString() const;
  virtual void readBin(FILE *file, const char *tag);
  virtual void saveBin(FILE *file, const char *tag) const;

  bool operator== (const Double &other) const {
    return equ(value, other.value);
  }

  int cmp(const Double &other) const {
    return cmp(value, other.value);
  }

  int cmp(double value) const {
    return cmp(this->value, value);
  }

  bool checkAboveZero(const char *name) const { return checkAboveZero(value, name); }

  static bool equ(double a, double b) {
    return fabs(a - b) < DBL_EPSILON;
  }

  static int cmp(double a, double b);
  static bool checkAboveZero(double value, const char *name);
  static double parse2(const std::string &str);
};

#endif
