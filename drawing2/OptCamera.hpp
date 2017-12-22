/*
* File:   OptCamera.hpp
* Author: darek
*
* Created on 23 November 2014, 22:49
*/
#ifndef QFRACTAL_DRAWING_OPTCAMERA_HPP
#define QFRACTAL_DRAWING_OPTCAMERA_HPP

#include "ISerializable.hpp"
#include "utils.hpp"

class OptCameraTransSpeed : public ISerializable {
public:
  float left_right; // above zero
  float up_down;    // above zero

  void  setDefault();

  virtual void readJson(const nlohmann::json &json, const char *tag);
  virtual nlohmann::json saveJson(const char *tag) const;

  virtual void readBin(FILE *file, const char *tag);
  virtual void saveBin(FILE *file, const char *tag) const;

  virtual void validate(const char *tag) const;
};

class OptCameraZoomSpeed : public ISerializable {
public:
  float without_ctrl; // above zero
  float with_ctrl;    // above zero

  void  setDefault();

  virtual void readJson(const nlohmann::json &json, const char *tag);
  virtual nlohmann::json saveJson(const char *tag) const;

  virtual void readBin(FILE *file, const char *tag);
  virtual void saveBin(FILE *file, const char *tag) const;

  virtual void validate(const char *tag) const;
};

class OptCameraRotate : public ISerializable {
public:
  float           sensitivity;
  float           speed;
  bool            enabled_mouse_rotate;

  void  setDefault();

  virtual void readJson(const nlohmann::json &json, const char *tag);
  virtual nlohmann::json saveJson(const char *tag) const;

  virtual void readBin(FILE *file, const char *tag);
  virtual void saveBin(FILE *file, const char *tag) const;

  virtual void validate(const char *tag) const;
};

class OptCamera : public ISerializable {
public:
  OptCameraTransSpeed   trans_speed;
  OptCameraZoomSpeed    zoom_speed;
  OptCameraRotate     rotate;

  void  setDefault();

  virtual void readJson(const nlohmann::json &json, const char *tag);
  virtual nlohmann::json saveJson(const char *tag) const;

  virtual void readBin(FILE *file, const char *tag);
  virtual void saveBin(FILE *file, const char *tag) const;

  virtual void validate(const char *tag) const;
};

#endif
