/*
 * File:   OptDisplay.hpp
 * Author: darek
 *
 * Created on 23 November 2014, 23:27
 */
#ifndef QFRACTAL_DRAWING_OPTDISPLAY_HPP
#define QFRACTAL_DRAWING_OPTDISPLAY_HPP

#include "ISerializable.hpp"
#include "utils.hpp"
#include "Color.hpp"

class OptDisplayGeom : public ISerializable {
public:
  unsigned int x, y, w, h;

  void  setDefault();

  virtual void readJson(const nlohmann::json &json, const char *tag);
  virtual nlohmann::json saveJson(const char *tag) const;

  virtual void readBin(FILE *file, const char *tag);
  virtual void saveBin(FILE *file, const char *tag) const;

  virtual void validate(const char *tag) const;
};

class OptDisplay : public ISerializable {
public:
  Color4f     background;
  OptDisplayGeom  disp_geom;

  void  setDefault();

  virtual void readJson(const nlohmann::json &json, const char *tag);
  virtual nlohmann::json saveJson(const char *tag) const;

  virtual void readBin(FILE *file, const char *tag);
  virtual void saveBin(FILE *file, const char *tag) const;

  virtual void validate(const char *tag) const;

  void apply() {
    _setBackground();
  }

private:
  void _setBackground();
};

#endif

