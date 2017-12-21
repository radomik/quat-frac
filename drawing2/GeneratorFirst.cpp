#include "stdafx.h"
#include "GeneratorFirst.hpp"
#include "GeneratorFirstIspc.hpp"
#include "Range.hpp"

int GeneratorFirst::computeCustom(FILE *out_file) {
  GeneratorFirstIspc gen(m_opt, out_file);
  return gen.run();
}

const char* GeneratorFirst::getName() const {
  return "GeneratorFirst (ISPC optimized)";
}

unsigned int GeneratorFirst::getDataItemSize() const {
  return VEC3F_BIN_SIZE + sizeof(RangeIter::DATA_TYPE);
}

unsigned int GeneratorFirst::getVersion() const {
  return 1;
}

const OptGenerator& GeneratorFirst::getOptions() const {
  return m_opt;
}
