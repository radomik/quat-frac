/*
* File:   OptGenerator.hpp
* Author: darek
*
* Created on 20 November 2014, 23:48
*/
#ifndef QFRACTAL_GENERATOR_OPTGENERATOR_HPP
#define QFRACTAL_GENERATOR_OPTGENERATOR_HPP

#include "FractalFuncInd.hpp"
#include "FractalFunc.hpp"
#include "ISerializable.hpp"
#include "Quat.hpp"
#include "Range.hpp"
#include "utils.hpp"

class Generator;

class OptGenerator : public ISerializable {

public: /// common fields
  // algorithm version
  const unsigned int  version;

  // buffer size (used by ispc version)
  unsigned int  buf_size;

  // whether multiply range minimum, maximum and step by PI
  bool      mul_range_by_pi;

  // initial constant for recursion function (4-element vector: [w, x, y, z])
  Quatf       initial;

  // minimum and maximum recursive function ieration count
  // allowing point to be stored in set
  RangeIter   iter;

  // maximum value of square of quaternion modulus
  // after which recursive function is stopped
  float       max_len_sqr;

  // index of selected recursive function (see: `quaternion -f' for list)
  fractal_func_ind_t  fun_index;

  /// Used range: version=1 -> {x,y,z}, version=2 -> {x,y,z,w}
  Range4Df range;

  /// Used when version=1
  float mod_q;

public:
  OptGenerator() : buf_size(0) { }

  virtual void readJson(const nlohmann::json &json, const char *tag);
  virtual nlohmann::json saveJson(const char *tag) const;
  virtual void readBin(FILE *file, const char *tag);
  virtual void saveBin(FILE *file, const char *tag) const;
  virtual void validate(const char *tag) const;

  virtual ~OptGenerator() { }

  Generator* getNewGenerator();
  DrawingCreator* getNewDrawingCreator() const;

  static const char *TAG_BASE_PATH;
  static const char *TAG_VERSION;
  static const char *TAG_BUF_SIZE;
  static const char *TAG_MUL_RANGE_BY_PI;
  static const char *TAG_INITIAL;
  static const char *TAG_MAX_LEN_SQR;
  static const char *TAG_FUN_INDEX;
  static const char *TAG_ITER;
  static const char *TAG_RANGE;
  static const char *TAG_MOD_Q;
  static const size_t MAX_BUF_SIZE;
};

#endif
