#include "stdafx.h"
#include "DrawMode.hpp"
#include "BinSerialization.hpp"

void DrawMode::parse(const std::string &str)
{
  if (!sys_strcasecmp(str.c_str(), "fill")) {
    this->value = DRAW_MODE_FILL;
    return;
  }
  if ((!sys_strcasecmp(str.c_str(), "lines")) || (!sys_strcasecmp(str.c_str(), "line"))) {
    this->value = DRAW_MODE_LINES;
    return;
  }
  if ((!sys_strcasecmp(str.c_str(), "points")) || (!sys_strcasecmp(str.c_str(), "point"))) {
    this->value = DRAW_MODE_POINTS;
    return;
  }
  Q_THROW(this, "Invalid value: '%s' Expected: { fill, lines, line, points, point } (case insensitive)", str.c_str());
}

std::string DrawMode::toString() const
{
  switch (this->value) {
    case DRAW_MODE_FILL:
      return std::string("fill");
    case DRAW_MODE_LINES:
      return std::string("lines");
    case DRAW_MODE_POINTS:
      return std::string("points");
    default:
      Q_THROW(this, "Invalid value: %d (expected one of {%d,%d,%d})",
        this->value, DRAW_MODE_FILL, DRAW_MODE_LINES, DRAW_MODE_POINTS);
  }
}

void DrawMode::readBin(FILE *file, const char *tag)
{
  BinSerialization::readValueAsOneByte(file, this->value, tag);
}

void DrawMode::saveBin(FILE *file, const char *tag) const
{
  BinSerialization::writeValueAsOneByte(file, this->value, tag);
}
