/*
* File:   Integer.hpp
* Author: darek
*
* Created on 20 November 2014, 14:43
*/
#ifndef QFRACTAL_INTEGER_HPP
#define QFRACTAL_INTEGER_HPP

#include "IPrimitive.hpp"
#include "utils.hpp"

class U32 : public IPrimitive {
public:
  unsigned int  value;

  bool operator== (const U32 &other) const {
    return equ(value, other.value);
  }

  int cmp(const U32 &other) const {
    return cmp(value, other.value);
  }

  int cmp(unsigned int value) const {
    return cmp(this->value, value);
  }

  virtual void parse(const std::string &str);
  virtual std::string toString() const;
  virtual void readBin(FILE *file, const char *tag);
  virtual void saveBin(FILE *file, const char *tag) const;

  static bool equ(unsigned int a, unsigned int b) {
    return a == b;
  }

  static int cmp(unsigned int a, unsigned int b) {
    return a - b;
  }

  static unsigned int parse2(const std::string &str);
  static std::string toString(unsigned int value) { return std::to_string(value); }

private:
  static unsigned int _st_strToUint(const std::string &str);
  static unsigned int _st_strToUint2(const std::string &str);
};

class U8 : public IPrimitive {
public:
  unsigned char value;

  bool operator== (const U8 &other) const {
    return equ(value, other.value);
  }

  int cmp(const U8 &other) const {
    return cmp(value, other.value);
  }

  int cmp(unsigned char value) const {
    return cmp(this->value, value);
  }

  virtual void parse(const std::string &str);
  virtual std::string toString() const;
  virtual void readBin(FILE *file, const char *tag);
  virtual void saveBin(FILE *file, const char *tag) const;

  static bool equ(unsigned char a, unsigned char b) {
    return a == b;
  }

  static int cmp(unsigned char a, unsigned char b) {
    return a - b;
  }

  static unsigned char parse2(const std::string &str);
  static std::string toString(unsigned char value) { return std::to_string(value); }
};

#endif

