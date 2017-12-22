/*
* File:   OptCameraPos.hpp
* Author: darek
*
* Created on 23 November 2014, 22:43
*/
#ifndef QFRACTAL_DRAWING_OPTCAMERAPOS_HPP
#define QFRACTAL_DRAWING_OPTCAMERAPOS_HPP

#include "ISerializable.hpp"
#include "utils.hpp"
#include "Vec.hpp"

class OptCameraPos : public ISerializable {
public:
  Vec3f pos;
  Vec3f target;
  Vec3f up;

  void setDefault();

  virtual void readJson(const nlohmann::json &json, const char *tag);
  virtual nlohmann::json saveJson(const char *tag) const;

  virtual void readBin(FILE *file, const char *tag);
  virtual void saveBin(FILE *file, const char *tag) const;

  virtual void validate(const char *tag) const;
};

#endif
