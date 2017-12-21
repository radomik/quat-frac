/*
* File:   GeneratorFirst.hpp
* Author: darek
*
* Created on 11 December 2014, 12:40
*/
#ifndef QFRACTAL_GENERATOR_GENERATORFIRST_HPP
#define QFRACTAL_GENERATOR_GENERATORFIRST_HPP

#include "OptGeneratorFirst.hpp"
#include "Generator.hpp"
#include "utils.hpp"

class GeneratorFirst : public Generator {
  const OptGeneratorFirst &m_opt;

public:
  GeneratorFirst(const OptGeneratorFirst &opt) : m_opt(opt) { }
  ~GeneratorFirst() { }

protected:
  virtual int computeCustom(FILE *out_file);

  virtual const char*  getName() const;
  virtual unsigned int getDataItemSize() const;

  virtual unsigned int getVersion() const;
  virtual const OptGenerator& getOptions() const;
};

#endif
