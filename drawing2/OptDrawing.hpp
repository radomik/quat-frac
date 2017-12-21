/*
* File:   OptDrawing.hpp
* Author: darek
*
* Created on 18 December 2014, 01:06
*/
#ifndef QFRACTAL_OPTDRAWING_HPP
#define QFRACTAL_OPTDRAWING_HPP

#include "ISerializable.hpp"
#include "utils.hpp"
#include "DrawMode.hpp"
#include "Vec.hpp"

class OptDrawing : public ISerializable {
public:
  Vec3f rotation;
  draw_mode_t draw_mode;
  float rotate_angle;

  void setDefault();
  void validate() const;

  virtual void readJson(const nlohmann::json &json, const char *tag);
  virtual nlohmann::json saveJson(const char *tag) const;
  virtual void readBin(FILE *file, const char *tag);
  virtual void saveBin(FILE *file, const char *tag) const;

  void apply() { _setDrawMode(); }

  void toggleDrawMode();

private:
  void _setDrawMode();
};

#endif

