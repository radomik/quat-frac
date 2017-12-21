/*
* File:   FractalDrawing.hpp
* Author: darek
*
* Created on 24 November 2014, 00:31
*/
#ifndef QFRACTAL_DRAWING_FRACTALDRAWING_HPP
#define QFRACTAL_DRAWING_FRACTALDRAWING_HPP

#include "utils.hpp"
#include "OptGenerator.hpp"
#include "OptDrawingCreator.hpp"
#include "gfx.hpp"
#include "DrawingCreator.hpp"

class FractalDrawing {
public:

  FractalDrawing() :
    m_opt_drawing_creator(NULL), m_drawing_creator(NULL) { }

  ~FractalDrawing();

  int create(
    FILE*           fractal_data_file,
    const OptGenerator&     opt_generator,
    const OptDrawingCreator&  opt_drawing_creator);

  void draw()      const {
    if (m_drawing_creator)
      m_drawing_creator->drawingConst().draw();
  }

  bool isCreated() const {
    return
      ((m_drawing_creator) &&
      (m_drawing_creator->drawingConst().isCreated()));
  }

  void toggleDisplist();

  OptDrawingCreator* drawingCreator() {
    return m_opt_drawing_creator;
  }

private:
  OptDrawingCreator*  m_opt_drawing_creator;
  DrawingCreator*   m_drawing_creator;

  void _deleteDrawingCreator();
};

#endif

