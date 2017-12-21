#include "stdafx.h"
#include "FloatingPoint.hpp"
#include "BinSerialization.hpp"

template<typename T>
inline int FloatingPoint::cmp(T threshold, T a, T b)
{
  const T res = a - b;
  if (res > threshold)  return 1;
  if (res < -threshold) return -1;
  return 0;
}

template<typename T>
static T stringToNumber(const std::string& str)
{
  std::stringstream stream(str);
  T result;
  stream >> result;
  if (stream.fail()) {
    Q_STTHROW("'%s' is not a valid %s", str.c_str(), typnam(result));
  }
  return result;
}

void Float::parse(const std::string &str)
{
  this->value = Float::parse2(str);
}

void Double::parse(const std::string &str)
{
  this->value = Double::parse2(str);
}

std::string Float::toString() const
{
  return std::to_string(this->value);
}

std::string Double::toString() const
{
  return std::to_string(this->value);
}

int Float::cmp(float a, float b)
{
  return FloatingPoint::cmp(FLT_EPSILON, a, b);
}

int Double::cmp(double a, double b) {
  return FloatingPoint::cmp(DBL_EPSILON, a, b);
}

bool Float::checkAboveZero(float value, const char *name)
{
  if (Float::cmp(value, 0.0f) <= 0) {
    fprintf(stderr, "%s = %f (Expected value above zero)\n", name, value);
    return false;
  }
  return true;
}

bool Double::checkAboveZero(double value, const char *name)
{
  if (Double::cmp(value, 0.0) <= 0) {
    fprintf(stderr, "%s = %lf (Expected value above zero)\n", name, value);
    return false;
  }
  return true;
}

float Float::parse2(const std::string &str)
{
  try {
    return stringToNumber<float>(str);
  } catch (const std::exception &e) {
    Q_THROW(this, "'%s' is not a valid float:\n%s", str.c_str(), e.what());
  }
}

double Double::parse2(const std::string &str)
{
  try {
    return stringToNumber<double>(str);
  } catch (const std::exception &e) {
    Q_THROW(this, "'%s' is not a valid double:\n%s", str.c_str(), e.what());
  }
}

void Double::readBin(FILE *file, const char *tag) {
  BinSerialization::readValue(file, this->value, tag);
}

void Float::readBin(FILE *file, const char *tag) {
  BinSerialization::readValue(file, this->value, tag);
}

void Double::saveBin(FILE *file, const char *tag) const {
  BinSerialization::writeValue(file, this->value, tag);
}

void Float::saveBin(FILE *file, const char *tag) const {
  BinSerialization::writeValue(file, this->value, tag);
}
