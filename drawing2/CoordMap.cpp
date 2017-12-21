#include "stdafx.h"
#include "CoordMap.hpp"
#include "BinSerialization.hpp"
#include "Name.hpp"

const uint8_t CoordMap::SIZE = 3;
const char* CoordMap::VALUES = "xyzw";

QuatInd CoordMap::_st_charToInd(char c)
{
  c = tolower(c);
  switch (c) {
    case 'x': return QuatIndX;
    case 'y': return QuatIndY;
    case 'z': return QuatIndZ;
    case 'w': return QuatIndW;
    default: return QuatIndCount;
  }
}

void CoordMap::parse(const std::string &str)
{
  if (str.size() != CoordMap::SIZE) {
    Q_THROW(this, "Expected exactly %d characters, %zu obtained: '%s'",
      CoordMap::SIZE, str.size(), str.c_str());
  }
  for (unsigned int i = 0; i < CoordMap::SIZE; i++) {
    QuatInd qi = _st_charToInd(str[i]);
    if (qi == QuatIndCount) {
      Q_THROW(this, "Each character must be in range {w,x,y,z} (case insensitive), obtained: '%s'", str.c_str());
    }
    this->value[i] = (unsigned char)qi;
  }
}

std::string CoordMap::toString() const
{
  char str[] = {
    CoordMap::VALUES[(unsigned int)this->value[0]],
    CoordMap::VALUES[(unsigned int)this->value[1]],
    CoordMap::VALUES[(unsigned int)this->value[2]],
    '\0'
  };
  return std::string(str);
}

void CoordMap::readBin(FILE *file, const char *tag)
{
  BinSerialization::readArray(file, this->value, CoordMap::SIZE, tag);
}

void CoordMap::saveBin(FILE *file, const char *tag) const
{
  BinSerialization::writeArray(file, this->value, CoordMap::SIZE, tag);
}
