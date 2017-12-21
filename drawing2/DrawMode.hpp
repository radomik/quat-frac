/*
 * File:   DrawMode.hpp
 * Author: darek
 *
 * Created on 23 November 2014, 23:16
 */
#ifndef QFRACTAL_DRAWING_DRAWMODE_HPP
#define QFRACTAL_DRAWING_DRAWMODE_HPP

#include "IPrimitive.hpp"
#include "utils.hpp"

enum draw_mode_t {
  DRAW_MODE_FILL = 0,
  DRAW_MODE_LINES,
  DRAW_MODE_POINTS
};

class DrawMode : public IPrimitive {
public:
  draw_mode_t value;

  virtual void parse(const std::string &str);
  virtual std::string toString() const;

  virtual void readBin(FILE *file, const char *tag);
  virtual void saveBin(FILE *file, const char *tag) const;
};

#endif

