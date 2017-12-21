#include "stdafx.h"
#include "Range.hpp"
#include "FloatingPoint.hpp"
#include "BinSerialization.hpp"

const RangeIter::DATA_TYPE RangeIter::MIN = 1;
const RangeIter::DATA_TYPE RangeIter::MAX = 255;
const RangeIter::DATA_TYPE RangeIter::INVALID = 0; // must be outside [MIN,MAX]

static const char *TAG_X = "x";
static const char *TAG_Y = "y";
static const char *TAG_Z = "z";
static const char *TAG_W = "w";
static const char *TAG_MIN = "min";
static const char *TAG_MAX = "max";
static const char *TAG_STEP = "step";

bool Range1Df::operator== (const Range1Df &other) const {
  return (Float::equ(this->min, other.min) &&
    Float::equ(this->max, other.max) &&
    Float::equ(this->step, other.step));
}

bool Range3Df::operator== (const Range3Df &other) const {
  return ((this->x == other.x) && (this->y == other.y) && (this->z == other.z));
}

bool Range4Df::operator== (const Range4Df &other) const {
  return ((this->x == other.x) && (this->y == other.y) && (this->z == other.z) && (this->w == other.w));
}

inline void Range1Df::mulByPi() {
  this->min *= (float)M_PI;
  this->max *= (float)M_PI;
  this->step *= (float)M_PI;
}

void Range3Df::mulByPi() {
  this->x.mulByPi();
  this->y.mulByPi();
  this->z.mulByPi();
}

void Range4Df::mulByPi() {
  this->x.mulByPi();
  this->y.mulByPi();
  this->z.mulByPi();
  this->w.mulByPi();
}

nlohmann::json Range1Df::saveJson(const char *tag) const {
  try {
    return nlohmann::json({
      { TAG_MIN, this->min },
      { TAG_MAX, this->max },
      { TAG_STEP, this->step }
    });
  } catch (const std::exception &e) {
    Q_THROW(this, "Error creating JSON '%s':\n%s", tag, e.what());
  }
}

nlohmann::json Range3Df::saveJson(const char *tag) const {
  try {
    return nlohmann::json({
      { TAG_X, this->x.saveJson(TAG_X) },
      { TAG_Y, this->y.saveJson(TAG_Y) },
      { TAG_Z, this->z.saveJson(TAG_Z) }
    });
  } catch (const std::exception &e) {
    Q_THROW(this, "Error creating JSON '%s':\n%s", tag, e.what());
  }
}

nlohmann::json Range4Df::saveJson(const char *tag) const {
  try {
    return nlohmann::json({
      { TAG_X, this->x.saveJson(TAG_X) },
      { TAG_Y, this->y.saveJson(TAG_Y) },
      { TAG_Z, this->z.saveJson(TAG_Z) },
      { TAG_W, this->w.saveJson(TAG_W) }
    });
  } catch (const std::exception &e) {
    Q_THROW(this, "Error creating JSON '%s':\n%s", tag, e.what());
  }
}

nlohmann::json RangeIter::saveJson(const char *tag) const {
  try {
    return nlohmann::json({
      { TAG_MIN, this->min },
      { TAG_MAX, this->max }
    });
  } catch (const std::exception &e) {
    Q_THROW(this, "Error creating JSON '%s':\n%s", tag, e.what());
  }
}

void Range1Df::validate(const char *tag) const {
  try {
    if (Float::cmp(this->step, 0.0f) <= 0) {
      Q_THROW(this, "Step [%f] must be positive", this->step);
    }
    if (Float::cmp(this->min, this->max) > 0) {
      Q_THROW(this, "Min [%f] should be below Max [%f]", this->min, this->max);
    }
  } catch (const std::exception &e) {
    Q_THROW(this, "Fail to validate '%s' (min=%f, max=%f, step=%f):\n%s",
      tag, this->min, this->max, this->step, e.what());
  }
}

void Range3Df::validate(const char *tag) const {
  try {
    this->x.validate(TAG_X);
    this->y.validate(TAG_Y);
    this->z.validate(TAG_Z);
  } catch (const std::exception &e) {
    Q_THROW(this, "Fail to validate '%s':\n%s", tag, e.what());
  }
}

void Range4Df::validate(const char *tag) const {
  try {
    this->x.validate(TAG_X);
    this->y.validate(TAG_Y);
    this->z.validate(TAG_Z);
    this->w.validate(TAG_W);
  } catch (const std::exception &e) {
    Q_THROW(this, "Fail to validate '%s':\n%s", tag, e.what());
  }
}

void RangeIter::validate(const char *tag) const {
  try {
    if (this->min > this->max) {
      Q_THROW(this, "Min [%d] should be below Max [%d]", this->min, this->max);
    }
    if (this->min < RangeIter::MIN || this->min > RangeIter::MAX || this->max < RangeIter::MIN || this->max > RangeIter::MAX) {
      Q_THROW(this, "Expected min, max to be in range [%d...%d]. Obtained min=%d, max=%d",
        RangeIter::MIN, RangeIter::MAX, this->min, this->max);
    }
  } catch (const std::exception &e) {
    Q_THROW(this, "Fail to validate '%s' (min=%d, max=%d):\n%s",
      tag, this->min, this->max, e.what());
  }
}

void Range1Df::readJson(const nlohmann::json &json, const char *tag) {
  try {
    this->min = json[TAG_MIN];
    this->max = json[TAG_MAX];
    this->step = json[TAG_STEP];
  } catch (const std::exception &e) {
    Q_THROW(this, "Error reading JSON '%s':\n%s", tag, e.what());
  }
  this->validate(tag);
}

void Range1Df::readBin(FILE *file, const char *tag) {
  try {
    BinSerialization::readValue(file, this->min, TAG_MIN);
    BinSerialization::readValue(file, this->max, TAG_MAX);
    BinSerialization::readValue(file, this->step, TAG_STEP);
  } catch (const std::exception &e) {
    Q_THROW(this, "Error reading BIN '%s':\n%s", tag, e.what());
  }
  this->validate(tag);
}

void Range3Df::readJson(const nlohmann::json &json, const char *tag) {
  try {
    this->x.readJson(json[TAG_X], TAG_X);
    this->y.readJson(json[TAG_Y], TAG_Y);
    this->z.readJson(json[TAG_Z], TAG_Z);
  } catch (const std::exception &e) {
    Q_THROW(this, "Error reading JSON '%s':\n%s", tag, e.what());
  }
  this->validate(tag);
}

void Range3Df::readBin(FILE *file, const char *tag) {
  try {
    this->x.readBin(file, TAG_X);
    this->y.readBin(file, TAG_Y);
    this->z.readBin(file, TAG_Z);
  } catch (const std::exception &e) {
    Q_THROW(this, "Error reading JSON '%s':\n%s", tag, e.what());
  }
  this->validate(tag);
}

void Range4Df::readJson(const nlohmann::json &json, const char *tag) {
  try {
    this->x.readJson(json[TAG_X], TAG_X);
    this->y.readJson(json[TAG_Y], TAG_Y);
    this->z.readJson(json[TAG_Z], TAG_Z);
    this->w.readJson(json[TAG_W], TAG_W);
  } catch (const std::exception &e) {
    Q_THROW(this, "Error reading JSON '%s':\n%s", tag, e.what());
  }
  this->validate(tag);
}

void Range4Df::readBin(FILE *file, const char *tag) {
  try {
    this->x.readBin(file, TAG_X);
    this->y.readBin(file, TAG_Y);
    this->z.readBin(file, TAG_Z);
    this->w.readBin(file, TAG_W);
  } catch (const std::exception &e) {
    Q_THROW(this, "Error reading JSON '%s':\n%s", tag, e.what());
  }
  this->validate(tag);
}

void RangeIter::readJson(const nlohmann::json &json, const char *tag) {
  try {
    this->min = json[TAG_MIN];
    this->max = json[TAG_MAX];
  } catch (const std::exception &e) {
    Q_THROW(this, "Error reading JSON '%s':\n%s", tag, e.what());
  }
  this->validate(tag);
}

void RangeIter::readBin(FILE *file, const char *tag) {
  try {
    BinSerialization::readValue(file, this->min, TAG_MIN);
    BinSerialization::readValue(file, this->max, TAG_MAX);
  } catch (const std::exception &e) {
    Q_THROW(this, "Error reading BIN '%s':\n%s", tag, e.what());
  }
  this->validate(tag);
}

void Range1Df::saveBin(FILE *file, const char *tag) const {
  try {
    BinSerialization::writeValue(file, this->min, TAG_MIN);
    BinSerialization::writeValue(file, this->max, TAG_MAX);
    BinSerialization::writeValue(file, this->step, TAG_STEP);
  } catch (const std::exception &e) {
    Q_THROW(this, "Error writing BIN '%s':\n%s", tag, e.what());
  }
}

void Range3Df::saveBin(FILE *file, const char *tag) const {
  try {
    this->x.saveBin(file, TAG_X);
    this->y.saveBin(file, TAG_Y);
    this->z.saveBin(file, TAG_Z);
  } catch (const std::exception &e) {
    Q_THROW(this, "Error writing BIN '%s':\n%s", tag, e.what());
  }
}

void Range4Df::saveBin(FILE *file, const char *tag) const {
  try {
    this->x.saveBin(file, TAG_X);
    this->y.saveBin(file, TAG_Y);
    this->z.saveBin(file, TAG_Z);
    this->w.saveBin(file, TAG_W);
  } catch (const std::exception &e) {
    Q_THROW(this, "Error writing BIN '%s':\n%s", tag, e.what());
  }
}

void RangeIter::saveBin(FILE *file, const char *tag) const {
  try {
    BinSerialization::writeValue(file, this->min, TAG_MIN);
    BinSerialization::writeValue(file, this->max, TAG_MAX);
  } catch (const std::exception &e) {
    Q_THROW(this, "Error writing BIN '%s':\n%s", tag, e.what());
  }
}
