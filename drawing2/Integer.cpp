#include "stdafx.h"
#include "Integer.hpp"
#include "BinSerialization.hpp"
#include <regex>

void U32::parse(const std::string &str) {
  this->value = U32::parse2(str);
}

void U8::parse(const std::string &str) {
  this->value = U8::parse2(str);
}

std::string U32::toString() const {
  return U32::toString(value);
}

std::string U8::toString() const {
  return U8::toString(value);
}

unsigned int U32::parse2(const std::string &str) {
  try {
    return U32::_st_strToUint(str);
  } catch (const std::exception &e) {
    log_debug("> %s", e.what());
    Q_STTHROW("'%s' is not a valid unsigned integer", str.c_str());
  }
}

unsigned char U8::parse2(const std::string &str) {
  try {
    unsigned int val_uint = U32::parse2(str);
    if (val_uint > 0xFF) throw std::out_of_range();
    return (unsigned char)unit_val;
  } catch (const std::exception &e) {
    log_debug("> %s", e.what());
    Q_STTHROW("'%s' is not a valid unsigned char number", str.c_str());
  }
}

void U32::readBin(FILE *file, const char *tag) {
  BinSerialization::readValue(file, this->value, tag);
}

void U8::readBin(FILE *file, const char *tag) {
  BinSerialization::readValue(file, this->value, tag);
}

void U32::saveBin(FILE *file, const char *tag) const {
  BinSerialization::writeValue(file, this->value, tag);
}

void U8::saveBin(FILE *file, const char *tag) const {
  BinSerialization::writeValue(file, this->value, tag);
}

unsigned int U32::_st_strToUint2(const std::string &str) {
  std::string maxUintStr = std::to_string(UINT_MAX);
  if (str.size() > maxUintStr.size()) { // must contain no more chars than in UINT_MAX converted to string
    Q_STTHROW("'%s' is too long (than: %s)", str.c_str(), maxUintStr.c_str());
  }

  const double doubleValue = Double::parse2(str);
  const double max = UINT_MAX;

  if (Double::cmp(doubleValue, max) > 0) { // must not be above UINT
    Q_STTHROW("'%s' <-> %lf is out of unsigned int range [0 ... %u]", str.c_str(), doubleValue, (unsigned int)max);
  }
  return (unsigned int)doubleValue;
}

unsigned int U32::_st_strToUint(const std::string &str) {
  static const char *regex_str = "\\d+";
  std::regex e(regex_str);
  if (! std::regex_match(str, e)) { // contains non-digits
    Q_STTHROW("'%s' does not match regex '%s'", str.c_str(), regex_str);
  }

  size_t firstNonZero = str.find_first_not_of('0');
  if (firstNonZero == std::string::npos) { // all zeros
    return 0;
  } else if (firstNonZero == 0) { // no leading zeros
    return U32::_st_strToUint2(str);
  } else { // trim leading zeros
    std::string str2 = str.substr(firstNonZero);
    return U32::_st_strToUint2(str2);
  }
}
