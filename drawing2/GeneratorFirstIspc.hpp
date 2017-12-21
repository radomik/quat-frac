/*
* File:   GeneratorFirstIspc.hpp
* Author: darek
*
* Created on 23 December 2014, 23:50
*/
#ifndef QFRACTAL_GENERATOR_GENERATORFIRSTISPC_HPP
#define QFRACTAL_GENERATOR_GENERATORFIRSTISPC_HPP

#include "utils.hpp"
#include "OptGeneratorFirst.hpp"
#include "GeneratorIspc.hpp"

#define DBG_ISPC 1

class GeneratorFirstIspc : public GeneratorIspc {
  const OptGeneratorFirst &m_opt;
  ispc::float3      *m_points;
  ispc::int32_t2      *m_indices;
public:

  GeneratorFirstIspc(const OptGeneratorFirst &opt, FILE *f_out);

  virtual ~GeneratorFirstIspc();

  virtual int run();

private:
  int _writeData(unsigned int point_index, size_t &written_count);

#ifdef DBG_ISPC
  void _printDebug(FILE *dbg_file, size_t &abs_index, size_t point_count);
#endif
};
#endif /* QFRACTAL_GENERATOR_GENERATORFIRSTISPC_HPP */

