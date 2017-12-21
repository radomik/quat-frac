/*
* File:   GeneratorSecondIspc.hpp
* Author: darek
*
* Created on 26 December 2014, 01:14
*/
#ifndef QFRACTAL_GENERATOR_GENERATORSECONDISPC_HPP
#define QFRACTAL_GENERATOR_GENERATORSECONDISPC_HPP

#include "utils.hpp"
#include "OptGeneratorSecond.hpp"
#include "GeneratorIspc.hpp"

class GeneratorSecondIspc : public GeneratorIspc {
  const OptGeneratorSecond  &m_opt;
  ispc::float4        *m_points;
  ispc::int32_t2        *m_indices;
public:

  GeneratorSecondIspc(const OptGeneratorSecond &opt, FILE *f_out);

  virtual ~GeneratorSecondIspc();

  virtual int run();

private:
  int _writeData(unsigned int point_index, size_t &written_count);
};
#endif /* QFRACTAL_GENERATOR_GENERATORSECONDISPC_HPP */


