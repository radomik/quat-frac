#include "stdafx.h"
#include "BinSerialization.hpp"
#include "FractalDrawing.hpp"

FractalDrawing::~FractalDrawing() {
  if (m_opt_drawing_creator) {
    //fprintf(stderr, "[DBG] %s @ %d [DEL] %p (OptDrawingCreator)\n", __FILE__, __LINE__, m_opt_drawing_creator);
    delete(m_opt_drawing_creator);
    m_opt_drawing_creator = NULL;
  }
  _deleteDrawingCreator();
}

void FractalDrawing::_deleteDrawingCreator() {
  if (m_drawing_creator)
  { // delete current creator and all current display objects if exists
    //fprintf(stderr, "FractalDrawing::_deleteDrawingCreator: Destroying display objects\n");
    //m_drawing_creator->destroyDisplayObjects();

    //fprintf(stderr, "[DBG] %s @ %d [DEL] %p (DrawingCreator)\n", __FILE__, __LINE__, m_drawing_creator);
    delete(m_drawing_creator);
    m_drawing_creator = NULL;
  }
}

int FractalDrawing::create(
    FILE*           fractal_data_file,
    const OptGenerator&     opt_generator,
    const OptDrawingCreator&  opt_drawing_creator)
{
  // delete current creator and all current display objects if exists
  _deleteDrawingCreator();

  // get drawing creator dependend on generator version
  m_drawing_creator = opt_generator.getNewDrawingCreator();

  // Create fractal drawing in m_drawing
  assert(m_drawing_creator != NULL);
  if (m_drawing_creator->create(fractal_data_file, opt_drawing_creator) < 0) {
    fprintf(stderr, "FractalDrawing: FractalDrawingCreator::create() failed\n");

    _deleteDrawingCreator();
    return -1;
  }

  // copy opt_drawing_creator
  if (m_opt_drawing_creator) {
    //fprintf(stderr, "[DBG] %s @ %d [DEL] %p (OptDrawingCreator)\n", __FILE__, __LINE__, m_opt_drawing_creator);
    delete(m_opt_drawing_creator);
  }

  m_opt_drawing_creator = OptDrawingCreator::newByCopy(opt_drawing_creator);

  if (!m_opt_drawing_creator) {
    fprintf(stderr, "FractalDrawing: Failed to copy opt_drawing_creator\n");
    _deleteDrawingCreator();
    return -1;
  }

  return 0;
}

void FractalDrawing::toggleDisplist() {
  m_drawing_creator->drawing().toggleDisplist();

  if (m_opt_drawing_creator)
    m_opt_drawing_creator->disp_list_ind = m_drawing_creator->drawing().getCurrentObjectInd();
  else
    fprintf(stderr, "[WARN] %s: m_opt_drawing_creator is NULL\n", __FUNCTION__);
}
