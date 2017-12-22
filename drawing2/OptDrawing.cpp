#include "stdafx.h"
#include "OptDrawing.hpp"
#include "FloatingPoint.hpp"
#include "Parser.hpp"
#include "BinSerialization.hpp"
#include "Name.hpp"
#include "Integer.hpp"
#include "gfx.hpp"

void OptDrawing::setDefault()
{
  this->rotation.set(0.0f, 330.0f, 0.0f);
  this->draw_mode = DRAW_MODE_FILL;
  this->rotate_angle = 5.0f;
}

void OptDrawing::validate() const
{
#error "TODO validate angle ranges"
}

void OptDrawing::readJson(const nlohmann::json &json, const char *tag) {
  const char *self_name = "OptDrawing";

  DrawMode o_draw_mode;
  Float o_rotate_angle;

  bool rd_rotation = false;
  bool rd_draw_mode = false;
  bool rd_rotate_angle = false;

  int ret;

  while (!parser.isEof()) {
    if (parser.parseLine() < 0)
      return -1;

    if (parser.isClosureEnd()) break;

    if (parser.isClosureBegin()) {
      if ((ret = parser.parseClosure(rotation, Name::rotation, &rd_rotation, self_name)) < 0) return -1;
      if ((ret > 0) && (rd_rotation)) continue;

      parser.unexpectedClosure(self_name);
      return -1;
    }
    else {
      if (parser.isToken()) {
        if ((ret = parser.parseToken(o_draw_mode, Name::draw_mode, &rd_draw_mode, self_name)) < 0) return -1;
        if ((ret > 0) && (rd_draw_mode)) { draw_mode = o_draw_mode.value; continue; }

        if ((ret = parser.parseToken(o_rotate_angle, Name::rotate_angle, &rd_rotate_angle, self_name)) < 0) return -1;
        if ((ret > 0) && (rd_rotate_angle)) {
          if ((o_rotate_angle.cmp(0.0f) < 0) || (o_rotate_angle.cmp(360.0f) > 0)) {
            fprintf(stderr, "%s: Invalid rotate_angle (expected value in range [0.0; 360.0]\n", self_name);
            return -1;
          }
          rotate_angle = o_rotate_angle.value; continue;
        }

        parser.unexpectedToken(self_name);
        return -1;
      }
    }
  }

  if ((parser.isFull()) && (!(rd_draw_mode && rd_rotate_angle && rd_rotation)))
  {
    fprintf(stderr, "%s: Partially parsed:\n\
Have{draw_mode ; rotate_angle ; rotation}={%d ; %d ; %d}\n", self_name,
        rd_draw_mode, rd_rotate_angle, rd_rotation);
    return -1;
  }

  return 0;
}

nlohmann::json OptDrawing::saveJson(const char *tag) const {
  DrawMode o_draw_mode;
  Float o_rotate_angle;

  if (parser.writeObject(rotation, Name::rotation) < 0) return -1;

  o_draw_mode.value = draw_mode;
  if (parser.writePrimitive(o_draw_mode, Name::draw_mode) < 0) return -1;

  o_rotate_angle.value = rotate_angle;
  if (parser.writePrimitive(o_rotate_angle, Name::rotate_angle) < 0) return -1;

  return 0;
}

void OptDrawing::readBin(FILE *file, const char *tag) {
  DrawMode o_draw_mode;

  if (rotation.readBin(f) < 0) return -1;

  o_draw_mode.value = draw_mode;
  if (o_draw_mode.readBin(f) < 0) return -1;

  if (BinSerialization::readValue<float>(f, rotate_angle) < 0) return -1;

  return 0;
}

void OptDrawing::saveBin(FILE *file, const char *tag) const {
  DrawMode o_draw_mode;

  if (rotation.saveBin(f) < 0) return -1;

  o_draw_mode.value = draw_mode;
  if (o_draw_mode.saveBin(f) < 0) return -1;

  if (BinSerialization::writeValue<float>(f, rotate_angle) < 0) return -1;

  return 0;
}

void OptDrawing::_setDrawMode() {
  GLenum gl_draw_mode;
  switch (draw_mode) {
  case DRAW_MODE_FILL:
    gl_draw_mode = GL_FILL; break;
  case DRAW_MODE_LINES:
    gl_draw_mode = GL_LINE; break;
  case DRAW_MODE_POINTS:
    gl_draw_mode = GL_POINT; break;
  default:
    return;
  }

  glPolygonMode(GL_FRONT_AND_BACK, gl_draw_mode);
}

void OptDrawing::toggleDrawMode() {
  switch (draw_mode) {
  case DRAW_MODE_FILL:
    draw_mode = DRAW_MODE_LINES; break;
  case DRAW_MODE_LINES:
    draw_mode = DRAW_MODE_POINTS; break;
  case DRAW_MODE_POINTS:
    draw_mode = DRAW_MODE_FILL; break;
  default:
    return;
  }

  _setDrawMode();
}

