/*
* File:   OptAppDrawing.hpp
* Author: darek
*
* Created on 26 November 2014, 16:12
*/
#ifndef QFRACTAL_DRAWING_OPTAPPDRAWING_HPP
#define QFRACTAL_DRAWING_OPTAPPDRAWING_HPP

#include "utils.hpp"
#include "ISerializable.hpp"
#include "OptCamera.hpp"
#include "OptCameraPos.hpp"
#include "OptDisplay.hpp"
#include "OptDrawingCreator.hpp"
#include "OptDrawing.hpp"

class OptAppDrawing : public ISerializable {
public:
  OptCamera       opt_camera;
  OptCameraPos      opt_camera_pos;
  OptDisplay        opt_display;
  OptDrawing        opt_drawing;
  OptDrawingCreator*    opt_drawing_creator;

  OptAppDrawing() : opt_drawing_creator(NULL) {  }
  ~OptAppDrawing();

  int     setDefault(unsigned int generator_version);

  virtual void readJson(const nlohmann::json &json, const char *tag);
  virtual nlohmann::json saveJson(const char *tag) const;

  virtual void readBin(FILE *file, const char *tag);
  virtual void saveBin(FILE *file, const char *tag) const;
  virtual void validate(const char *tag) const;
};

#endif




