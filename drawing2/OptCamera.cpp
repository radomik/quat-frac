#include "stdafx.h"
#include "OptCamera.hpp"
#include "Parser.hpp"
#include "BinSerialization.hpp"
#include "Name.hpp"
#include "FloatingPoint.hpp"
#include "TwoStateVariable.hpp"

inline void OptCameraTransSpeed::setDefault() {
  left_right = 0.0025f;
  up_down    = 0.0300f;
}

inline void OptCameraZoomSpeed::setDefault() {
  without_ctrl = 0.080f;
  with_ctrl    = 0.009f;
}

inline void OptCameraRotate::setDefault() {
  sensitivity =  1000.0f;
  speed       =  3.00f;
  enabled_mouse_rotate = true;
}

void OptCamera::setDefault() {
  trans_speed.setDefault();
  zoom_speed.setDefault();
  rotate.setDefault();
}

void OptCameraTransSpeed::readJson(const nlohmann::json &json, const char *tag) {
  const char *self_name = "OptCameraTransSpeed";

  Float val_float;

  bool rd_left_right  = false;
  bool rd_up_down   = false;

  int ret;

  while (! parser.isEof()) {
    if (parser.parseLine() < 0)
      return -1;

    if (parser.isClosureEnd()) break;

    if (parser.isClosureBegin()) {
      parser.unexpectedClosure(self_name);
      return -1;
    }
    else {
      if (parser.isToken()) {
        if ((ret=parser.parseToken(val_float, Name::left_right, &rd_left_right, self_name)) < 0) return -1;
        if ((ret > 0) && (rd_left_right)) {
          if (! val_float.checkAboveZero(Name::left_right)) return -1;
          left_right = val_float.value; continue;
        }

        if ((ret=parser.parseToken(val_float, Name::up_down, &rd_up_down, self_name)) < 0) return -1;
        if ((ret > 0) && (rd_up_down)) {
          if (! val_float.checkAboveZero(Name::up_down)) return -1;
          up_down = val_float.value; continue;
        }

        parser.unexpectedToken(self_name);
        return -1;
      }
    }
  }

  if ((parser.isFull()) && (! (rd_left_right && rd_up_down)))
  {
    fprintf(stderr, "%s: Partially parsed:\n\
Have{left_right,up_down}={%d,%d}\n", self_name, rd_left_right, rd_up_down);
    return -1;
  }

  return 0;
}

void OptCameraZoomSpeedreadJson(const nlohmann::json &json, const char *tag) {
  const char *self_name = "OptCameraZoomSpeed";

  Float val_float;

  bool rd_without_ctrl  = false;
  bool rd_with_ctrl   = false;

  int ret;

  while (! parser.isEof()) {
    if (parser.parseLine() < 0)
      return -1;

    if (parser.isClosureEnd()) break;

    if (parser.isClosureBegin()) {
      parser.unexpectedClosure(self_name);
      return -1;
    }
    else {
      if (parser.isToken()) {
        if ((ret=parser.parseToken(val_float, Name::without_ctrl, &rd_without_ctrl, self_name)) < 0) return -1;
        if ((ret > 0) && (rd_without_ctrl)) {
          if (! val_float.checkAboveZero(Name::without_ctrl)) return -1;
          without_ctrl = val_float.value; continue;
        }

        if ((ret=parser.parseToken(val_float, Name::with_ctrl, &rd_with_ctrl, self_name)) < 0) return -1;
        if ((ret > 0) && (rd_with_ctrl)) {
          if (! val_float.checkAboveZero(Name::with_ctrl)) return -1;
          with_ctrl = val_float.value; continue;
        }

        parser.unexpectedToken(self_name);
        return -1;
      }
    }
  }

  if ((parser.isFull()) && (! (rd_without_ctrl && rd_with_ctrl)))
  {
    fprintf(stderr, "%s: Partially parsed:\n\
Have{without_ctrl,with_ctrl}={%d,%d}\n", self_name, rd_without_ctrl, rd_with_ctrl);
    return -1;
  }

  return 0;
}

void OptCameraRotate::readJson(const nlohmann::json &json, const char *tag) {
  const char *self_name = "OptCameraRotate";

  YesNo o_enabled_mouse_rotate;
  Float val_float;

  bool rd_sensitivity         = false;
  bool rd_speed           = false;
  bool rd_enabled_mouse_rotate    = false;

  int ret;

  while (! parser.isEof()) {
    if (parser.parseLine() < 0)
      return -1;

    if (parser.isClosureEnd()) break;

    if (parser.isClosureBegin()) {
      parser.unexpectedClosure(self_name);
      return -1;
    }
    else {
      if (parser.isToken()) {
        if ((ret=parser.parseToken(val_float, Name::sensitivity, &rd_sensitivity, self_name)) < 0) return -1;
        if ((ret > 0) && (rd_sensitivity)) {
          if (! val_float.checkAboveZero(Name::sensitivity)) return -1;
          sensitivity = val_float.value; continue;
        }
        if ((ret=parser.parseToken(val_float, Name::speed, &rd_speed, self_name)) < 0) return -1;
        if ((ret > 0) && (rd_speed)) {
          if (! val_float.checkAboveZero(Name::speed)) return -1;
          speed = val_float.value; continue;
        }
        if ((ret=parser.parseToken(o_enabled_mouse_rotate, Name::enabled_mouse_rotate, &rd_enabled_mouse_rotate, self_name)) < 0) return -1;
        if ((ret > 0) && (rd_enabled_mouse_rotate)) { enabled_mouse_rotate = o_enabled_mouse_rotate.value; continue; }

        parser.unexpectedToken(self_name);
        return -1;
      }
    }
  }

  if ((parser.isFull()) &&
    (! (rd_sensitivity && rd_speed && rd_enabled_mouse_rotate)))
  {
    fprintf(stderr, "%s: Partially parsed:\n\
Have{sensitivity ; speed ; enabled_mouse_rotate}=\n\
{%d ; %d ; %d}\n", self_name,
      rd_sensitivity, rd_speed, rd_enabled_mouse_rotate);
    return -1;
  }

  return 0;
}

void OptCamera::readJson(const nlohmann::json &json, const char *tag) {
  const char *self_name = "OptCamera";

  bool rd_trans_speed = false;
  bool rd_zoom_speed  = false;
  bool rd_rotate      = false;

  int ret;

  while (! parser.isEof()) {
    if (parser.parseLine() < 0)
      return -1;

    if (parser.isClosureEnd()) break;

    if (parser.isClosureBegin()) {
      if ((ret=parser.parseClosure(trans_speed, Name::trans_speed, &rd_trans_speed, self_name)) < 0) return -1;
      if ((ret > 0) && (rd_trans_speed)) continue;

      if ((ret=parser.parseClosure(zoom_speed, Name::zoom_speed, &rd_zoom_speed, self_name)) < 0) return -1;
      if ((ret > 0) && (rd_zoom_speed)) continue;

      if ((ret=parser.parseClosure(rotate, Name::rotate, &rd_rotate, self_name)) < 0) return -1;
      if ((ret > 0) && (rd_rotate)) continue;

      parser.unexpectedClosure(self_name);
      return -1;
    }
    else {
      if (parser.isToken()) {
        parser.unexpectedToken(self_name);
        return -1;
      }
    }
  }

  if ((parser.isFull()) && (! (rd_trans_speed && rd_zoom_speed && rd_rotate)))
  {
    fprintf(stderr, "%s: Partially parsed:\n\
Have{trans_speed,zoom_speed,rotate}={%d,%d,%d}\n", self_name,
      rd_trans_speed, rd_zoom_speed, rd_rotate);
    return -1;
  }

  return 0;
}

nlohmann::json OptCameraTransSpeed::saveJson(const char *tag) const {
  Float val_float;

  val_float.value = left_right;
  if (parser.writePrimitive(val_float, Name::left_right) < 0) return -1;

  val_float.value = up_down;
  if (parser.writePrimitive(val_float, Name::up_down) < 0) return -1;

  return 0;
}

nlohmann::json OptCameraZoomSpeed::saveJson(const char *tag) const {
  Float val_float;

  val_float.value = without_ctrl;
  if (parser.writePrimitive(val_float, Name::without_ctrl) < 0) return -1;

  val_float.value = with_ctrl;
  if (parser.writePrimitive(val_float, Name::with_ctrl) < 0) return -1;

  return 0;
}

nlohmann::json OptCameraRotate::saveJson(const char *tag) const {
  Float val_float;
  YesNo o_enabled_mouse_rotate;

  val_float.value = sensitivity;
  if (parser.writePrimitive(val_float, Name::sensitivity) < 0) return -1;

  val_float.value = speed;
  if (parser.writePrimitive(val_float, Name::speed) < 0) return -1;

  o_enabled_mouse_rotate.value = enabled_mouse_rotate;
  if (parser.writePrimitive(o_enabled_mouse_rotate, Name::enabled_mouse_rotate) < 0) return -1;

  return 0;
}

nlohmann::json OptCamera::saveJson(const char *tag) const {
  if (parser.writeObject(trans_speed, Name::trans_speed) < 0) return -1;
  if (parser.writeObject(zoom_speed, Name::zoom_speed) < 0) return -1;
  if (parser.writeObject(rotate, Name::rotate) < 0) return -1;

  return 0;
}

void OptCameraTransSpeed::readBin(FILE *file, const char *tag) {
  if (BinSerialization::readValue<float>(f, left_right) < 0) return -1;
  if (BinSerialization::readValue<float>(f, up_down) < 0) return -1;
  return 0;
}

void OptCameraZoomSpeed::readBin(FILE *file, const char *tag) {
  if (BinSerialization::readValue<float>(f, without_ctrl) < 0) return -1;
  if (BinSerialization::readValue<float>(f, with_ctrl) < 0) return -1;
  return 0;
}

void OptCameraRotate::readBin(FILE *file, const char *tag) {
  if (BinSerialization::readValue<float>(f, sensitivity) < 0) return -1;
  if (BinSerialization::readValue<float>(f, speed) < 0) return -1;
  if (BinSerialization::readBool(f, enabled_mouse_rotate) < 0) return -1;
  return 0;
}

void OptCamera::readBin(FILE *file, const char *tag) {
  if (trans_speed.readBin(f) < 0) return -1;
  if (zoom_speed.readBin(f) < 0) return -1;
  if (rotate.readBin(f) < 0) return -1;
  return 0;
}

void OptCameraTransSpeed::saveBin(FILE *file, const char *tag) const {
  if (BinSerialization::writeValue<float>(f, left_right) < 0) return -1;
  if (BinSerialization::writeValue<float>(f, up_down) < 0) return -1;
  return 0;
}

void OptCameraZoomSpeed::saveBin(FILE *file, const char *tag) const {
  if (BinSerialization::writeValue<float>(f, without_ctrl) < 0) return -1;
  if (BinSerialization::writeValue<float>(f, with_ctrl) < 0) return -1;
  return 0;
}

void OptCameraRotate::saveBin(FILE *file, const char *tag) const {
  if (BinSerialization::writeValue<float>(f, sensitivity) < 0) return -1;
  if (BinSerialization::writeValue<float>(f, speed) < 0) return -1;
  if (BinSerialization::writeBool(f, enabled_mouse_rotate) < 0) return -1;
  return 0;
}

void OptCamera::saveBin(FILE *file, const char *tag) const {
  if (trans_speed.saveBin(f) < 0) return -1;
  if (zoom_speed.saveBin(f) < 0) return -1;
  if (rotate.saveBin(f) < 0) return -1;
  return 0;
}
