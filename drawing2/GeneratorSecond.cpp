#include "stdafx.h"
#include "GeneratorSecond.hpp"
#include "GeneratorSecondIspc.hpp"
#include "Range.hpp"

int GeneratorSecond::computeCustom(FILE *out_file) {
  GeneratorSecondIspc gen(m_opt, out_file);
  return gen.run();
}

const char* GeneratorSecond::getName() const {
  return "GeneratorSecond (ISPC optimized)";
}

unsigned int GeneratorSecond::getDataItemSize() const {
  return QUATF_BIN_SIZE + sizeof(RangeIter::DATA_TYPE);
}

unsigned int GeneratorSecond::getVersion() const {
  return 2;
}

const OptGenerator& GeneratorSecond::getOptions() const {
  return m_opt;
}
