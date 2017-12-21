/*
* File:   GeneratorIspc.hpp
* Author: darek
*
* Created on 27 December 2014, 17:41
*/
#ifndef QFRACTAL_GENERATOR_GENERATORISPC_HPP
#define QFRACTAL_GENERATOR_GENERATORISPC_HPP

#include "utils.hpp"
#include "OptGenerator.hpp"

class GeneratorIspc {
public:
  static const unsigned int MIN_BUF_SIZE;
  static const unsigned int MAX_BUF_SIZE;
  static const unsigned int DEFAULT_BUF_SIZE;

  GeneratorIspc(FILE *f_out, const OptGenerator &_opt, size_t points_item_size); // DO NOT remove '_' from _opt param

  virtual ~GeneratorIspc() { }
  virtual int run() = 0;

protected:
  FILE            *m_f_out;
  unsigned int        m_point_buf_count;

private:
  void _initPointBufCount(size_t buf_size, size_t points_item_size);
};
#endif /* QFRACTAL_GENERATOR_GENERATORISPC_HPP */

