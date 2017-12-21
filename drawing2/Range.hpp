/*
* File:   Range.hpp
* Author: darek
*
* Created on 20 November 2014, 00:36
*/
#ifndef QFRACTAL_RANGE_HPP
#define QFRACTAL_RANGE_HPP

#include "ISerializable.hpp"
#include "utils.hpp"

class Range1Df : public ISerializable {
public:
  float min, max, step;

  virtual void readJson(const nlohmann::json &json);
  virtual nlohmann::json saveJson() const;
  virtual void readBin(FILE *file);
  virtual void saveBin(FILE *file) const;

  bool operator ==(const Range1Df &other) const;

  void mulByPi();

  void validate() const;
};

class Range3Df : public ISerializable {
public:
  Range1Df x, y, z;

  virtual void readJson(const nlohmann::json &json);
  virtual nlohmann::json saveJson() const;
  virtual void readBin(FILE *file);
  virtual void saveBin(FILE *file) const;

  bool operator ==(const Range3Df &other) const;

  void mulByPi();

  void validate() const;
};

class Range4Df : public ISerializable {
public:
  Range1Df x, y, z, w;

  virtual void readJson(const nlohmann::json &json);
  virtual nlohmann::json saveJson() const;
  virtual void readBin(FILE *file);
  virtual void saveBin(FILE *file) const;

  bool operator ==(const Range4Df &other) const;

  void mulByPi();

  void validate() const;
};


class RangeIter : public ISerializable {
public:
  typedef unsigned char DATA_TYPE;
  static const DATA_TYPE MIN;
  static const DATA_TYPE MAX;
  static const DATA_TYPE INVALID;

  DATA_TYPE min, max;

  virtual void readJson(const nlohmann::json &json);
  virtual nlohmann::json saveJson() const;
  virtual void readBin(FILE *file);
  virtual void saveBin(FILE *file) const;

  bool operator ==(const RangeIter &other) const {
    return (min == other.min) && (max == other.max);
  }

  void validate() const;
};

#endif
