/*
* File:   GeneratorSecond.hpp
* Author: darek
*
* Created on 11 December 2014, 12:41
*/
#ifndef QFRACTAL_GENERATOR_GENERATORSECOND_HPP
#define QFRACTAL_GENERATOR_GENERATORSECOND_HPP

#include "OptGeneratorSecond.hpp"
#include "Generator.hpp"
#include "utils.hpp"

class GeneratorSecond : public Generator {
  const OptGeneratorSecond &m_opt;

public:
  GeneratorSecond(const OptGeneratorSecond &opt) : m_opt(opt) { }
  ~GeneratorSecond() { }

protected:
  virtual int computeCustom(FILE *out_file);

  virtual const char*  getName() const;
  virtual unsigned int getDataItemSize() const;

  virtual unsigned int getVersion() const;
  virtual const OptGenerator& getOptions() const;
};
#endif
