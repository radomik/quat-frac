#include "stdafx.h"
#include "Color.hpp"
#include "FloatingPoint.hpp"
#include "BinSerialization.hpp"
#include "GeomMath.hpp"

bool Color4f::operator ==(const Color4f& c) const {
  return vec4::equals(dataConst(), c.dataConst());
}

void Color4f::validate(const char *tag) const {
  try {
    for (unsigned int i = 0; i < Color4IndCount; i++) {
      if (!validChannel((*this)[i])) {
        Q_THROW(this, "Each channel must be in range [0.0f ... 1.0f]");
      }
    }
  } catch (const std::exception &e) {
    Q_THROW(this, "Fail to validate '%s' (r=%f, g=%f, b=%f, a=%f):\n%s",
      tag, this->rConst(), this->gConst(), this->bConst(), this->aConst(), e.what());
  }
}

void Color4f::readJson(const nlohmann::json &json, const char *tag) {
  try {
    this->r() = json["r"];
    this->g() = json["g"];
    this->b() = json["b"];
    this->a() = json["a"];
  } catch (const std::exception &e) {
    Q_THROW(this, "Error reading JSON '%s':\n%s", tag, e.what());
  }
  this->validate(tag);
}

nlohmann::json Color4f::saveJson(const char *tag) const {
  try {
    return nlohmann::json({
      { "r", this->rConst() },
      { "g", this->gConst() },
      { "b", this->bConst() },
      { "a", this->aConst() }
    });
  } catch (const std::exception &e) {
    Q_THROW(this, "Error creating JSON '%s':\n%s", tag, e.what());
  }
}

void Color4f::readBin(FILE *file, const char *tag) {
  try {
    BinSerialization::readArray(file, m_val.v, sizeof(m_val.v)/sizeof(m_val.v[0]), tag);
  } catch (const std::exception &e) {
    Q_THROW(this, "Error reading BIN '%s':\n%s", tag, e.what());
  }
}

void Color4f::saveBin(FILE *file, const char *tag) const {
  try {
    BinSerialization::writeArray(file, m_val.v, sizeof(m_val.v)/sizeof(m_val.v[0]), tag);
  } catch (const std::exception &e) {
    Q_THROW(this, "Error writing BIN '%s':\n%s", tag, e.what());
  }
}

bool Color4f::validChannel(float value) {
  return ((Float::cmp(value, 0.0f) >= 0) && (Float::cmp(value, 1.0f) <= 0));
}

int Color4f::_invalChannel(const char *channel, float value) {
  fprintf(stderr, "Color4f: Invalid %s channel value: %f (expected [0.0, 1.0])\n",
    channel, value);
  return -1;
}
