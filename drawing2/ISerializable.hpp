/*
* File:   ISerializable.hpp
* Author: darek
*
* Created on 20 November 2014, 00:33
*/
#ifndef QFRACTAL_ISERIALIZABLE_HPP
#define QFRACTAL_ISERIALIZABLE_HPP

#include "utils.hpp"

class ISerializable {
public:
  virtual void readJson(const nlohmann::json &json, const char *tag) = 0;
  virtual nlohmann::json saveJson(const char *tag) const = 0;

  virtual void readBin(FILE *file, const char *tag) = 0;
  virtual void saveBin(FILE *file, const char *tag) const = 0;
};

#endif

