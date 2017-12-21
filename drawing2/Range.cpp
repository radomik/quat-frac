#include "stdafx.h"
#include "Range.hpp"
#include "FloatingPoint.hpp"
#include "BinSerialization.hpp"

const RangeIter::DATA_TYPE RangeIter::MIN = 1;
const RangeIter::DATA_TYPE RangeIter::MAX = 255;
const RangeIter::DATA_TYPE RangeIter::INVALID = 0; // must be outside [MIN,MAX]

bool Range1Df::operator== (const Range1Df &other) const {
  return (Float::equ(min, other.min) &&
    Float::equ(max, other.max) &&
    Float::equ(step, other.step));
}

bool Range3Df::operator== (const Range3Df &other) const {
  return ((x == other.x) && (y == other.y) && (z == other.z));
}

bool Range4Df::operator== (const Range4Df &other) const {
  return ((x == other.x) && (y == other.y) && (z == other.z) && (w == other.w));
}

inline void Range1Df::mulByPi() {
  min *= (float)M_PI;
  max *= (float)M_PI;
  step *= (float)M_PI;
}

void Range3Df::mulByPi() {
  x.mulByPi();
  y.mulByPi();
  z.mulByPi();
}

void Range4Df::mulByPi() {
  x.mulByPi();
  y.mulByPi();
  z.mulByPi();
  w.mulByPi();
}

nlohmann::json Range1Df::saveJson() const {
  try {
    return nlohmann::json({
      { "min", min },
      { "max", max },
      { "step", step }
    });
  } catch (const std::exception &e) {
    Q_THROW(this, "Error creating JSON:\n%s", e.what());
  }
}

nlohmann::json Range3Df::saveJson() const {
  try {
    return nlohmann::json({
      { "x", this->x.saveJson() },
      { "y", this->y.saveJson() },
      { "z", this->z.saveJson() }
    });
  } catch (const std::exception &e) {
    Q_THROW(this, "Error creating JSON:\n%s", e.what());
  }
}

nlohmann::json Range4Df::saveJson() const {
  try {
    return nlohmann::json({
      { "x", this->x.saveJson() },
      { "y", this->y.saveJson() },
      { "z", this->z.saveJson() },
      { "w", this->w.saveJson() }
    });
  } catch (const std::exception &e) {
    Q_THROW(this, "Error creating JSON:\n%s", e.what());
  }
}

nlohmann::json RangeIter::saveJson() const {
  try {
    return nlohmann::json({
      { "min", this->min },
      { "max", this->max }
    });
  } catch (const std::exception &e) {
    Q_THROW(this, "Error creating JSON:\n%s", e.what());
  }
}

void Range1Df:validate() const {
  if (Float::cmp(this->step, 0.0f) <= 0) {
    Q_THROW(this, "Step [%f] must be positive", this->step);
  }
  if (Float::cmp(this->min, this->max) > 0) {
    Q_THROW(this, "Min [%f] should be below Max [%f]", this->min, this->max);
  }
}

void Range3Df:validate() const {
  try { this->x.validate(); }
  catch (const std::exception &e) {
    Q_THROW(this, "Error validating range X:\n%s", e.what());
  }
  try { this->y.validate(); }
  catch (const std::exception &e) {
    Q_THROW(this, "Error validating range Y:\n%s", e.what());
  }
  try { this->z.validate(); }
  catch (const std::exception &e) {
    Q_THROW(this, "Error validating range Z:\n%s", e.what());
  }
}

void Range4Df:validate() const {
  try { this->x.validate(); }
  catch (const std::exception &e) {
    Q_THROW(this, "Error validating range X:\n%s", e.what());
  }
  try { this->y.validate(); }
  catch (const std::exception &e) {
    Q_THROW(this, "Error validating range Y:\n%s", e.what());
  }
  try { this->z.validate(); }
  catch (const std::exception &e) {
    Q_THROW(this, "Error validating range Z:\n%s", e.what());
  }
  try { this->w.validate(); }
  catch (const std::exception &e) {
    Q_THROW(this, "Error validating range W:\n%s", e.what());
  }
}

void RangeIter:validate() const {
  if (this->min > this->max) {
    Q_THROW(this, "Min [%d] should be below Max [%d]", this->min, this->max);
  }
  if (this->min < RangeIter::MIN || this->min > RangeIter::MAX || this->max < RangeIter::MIN || this->max > RangeIter::MAX) {
    Q_THROW(this, "Expected min, max to be in range [%d...%d]. Obtained min=%, max=%d",
      RangeIter::MIN, RangeIter::MAX, this->min, this->max);
  }
}
void Range1Df::readJson(const nlohmann::json &json) {
  try {
    this->min = json["min"];
    this->max = json["max"];
    this->step = json["step"];
  } catch (const std::exception &e) {
    Q_THROW(this, "Error reading JSON:\n%s", e.what());
  }
  try {
    this->validate();
  } catch (const std::exception &e) {
    Q_THROW(this, "Fail to validate (min=%f, max=%f, step=%f):\n%s",
      this->min, this->max, this->step, e.what());
  }
}

void Range1Df::readBin(FILE *file) {
  try {
    BinSerialization::readValue<float>(file, min, "min");
    BinSerialization::readValue<float>(file, max, "max");
    BinSerialization::readValue<float>(file, step, "step");
  } catch (const std::exception &e) {
    Q_THROW(this, "Error reading BIN:\n%s", e.what());
  }
  try {
    this->validate();
  } catch (const std::exception &e) {
    Q_THROW(this, "Fail to validate (min=%f, max=%f, step=%f):\n%s",
      this->min, this->max, this->step, e.what());
  }
}

void Range3Df::readJson(const nlohmann::json &json) {
  try {
    this->x.readJson(json["x"]);
  } catch (const std::exception &e) {
    Q_THROW(this, "Error reading range X:\n%s", e.what());
  }
  try {
    this->y.readJson(json["y"]);
  } catch (const std::exception &e) {
    Q_THROW(this, "Error reading range Y:\n%s", e.what());
  }
  try {
    this->z.readJson(json["z"]);
  } catch (const std::exception &e) {
    Q_THROW(this, "Error reading range Z:\n%s", e.what());
  }
}

void Range3Df::readBin(FILE *file) {
  try {
    this->x.readBin(file);
  } catch (const std::exception &e) {
    Q_THROW(this, "Error reading range X:\n%s", e.what());
  }
  try {
    this->y.readBin(file);
  } catch (const std::exception &e) {
    Q_THROW(this, "Error reading range Y:\n%s", e.what());
  }
  try {
    this->z.readBin(file);
  } catch (const std::exception &e) {
    Q_THROW(this, "Error reading range Z:\n%s", e.what());
  }
}

void Range4Df::readJson(const nlohmann::json &json) {
  try {
    this->x.readJson(json["x"]);
  } catch (const std::exception &e) {
    Q_THROW(this, "Error reading range X:\n%s", e.what());
  }
  try {
    this->y.readJson(json["y"]);
  } catch (const std::exception &e) {
    Q_THROW(this, "Error reading range Y:\n%s", e.what());
  }
  try {
    this->z.readJson(json["z"]);
  } catch (const std::exception &e) {
    Q_THROW(this, "Error reading range Z:\n%s", e.what());
  }
  try {
    this->w.readJson(json["w"]);
  } catch (const std::exception &e) {
    Q_THROW(this, "Error reading range W:\n%s", e.what());
  }
}

void Range4Df::readBin(FILE *file) {
  try {
    this->x.readBin(file);
  } catch (const std::exception &e) {
    Q_THROW(this, "Error reading range X:\n%s", e.what());
  }
  try {
    this->y.readBin(file);
  } catch (const std::exception &e) {
    Q_THROW(this, "Error reading range Y:\n%s", e.what());
  }
  try {
    this->z.readBin(file);
  } catch (const std::exception &e) {
    Q_THROW(this, "Error reading range Z:\n%s", e.what());
  }
  try {
    this->w.readBin(file);
  } catch (const std::exception &e) {
    Q_THROW(this, "Error reading range W:\n%s", e.what());
  }
}

void RangeIter::readJson(const nlohmann::json &json) {
  try {
    this->min = json["min"];
    this->max = json["max"];
  } catch (const std::exception &e) {
    Q_THROW(this, "Error reading JSON:\n%s", e.what());
  }
  try {
    this->validate();
  } catch (const std::exception &e) {
    Q_THROW(this, "Fail to validate (min=%d, max=%d):\n%s",
      this->min, this->max, e.what());
  }
}

void RangeIter::readBin(FILE *file) {
  try {
    BinSerialization::readValue<RangeIter::DATA_TYPE>(file, min, "min");
    BinSerialization::readValue<RangeIter::DATA_TYPE>(file, max, "max");
  } catch (const std::exception &e) {
    Q_THROW(this, "Error reading BIN:\n%s", e.what());
  }
  try {
    this->validate();
  } catch (const std::exception &e) {
    Q_THROW(this, "Fail to validate (min=%d, max=%d):\n%s",
      this->min, this->max, e.what());
  }
}

void Range1Df::saveBin(FILE *file) const {
  try {
    BinSerialization::writeValue<float>(file, min, "min");
    BinSerialization::writeValue<float>(file, max, "max");
    BinSerialization::writeValue<float>(file, step, "step");
  } catch (const std::exception &e) {
    Q_THROW(this, "Error writing BIN:\n%s", e.what());
  }
}

void Range3Df::saveBin(FILE *file) const {
  try {
    this->x.saveBin(file);
  } catch (const std::exception &e) {
    Q_THROW(this, "Error writing range X:\n%s", e.what());
  }
  try {
    this->y.saveBin(file);
  } catch (const std::exception &e) {
    Q_THROW(this, "Error writing range Y:\n%s", e.what());
  }
  try {
    this->z.saveBin(file);
  } catch (const std::exception &e) {
    Q_THROW(this, "Error writing range Z:\n%s", e.what());
  }
}

void Range4Df::saveBin(FILE *file) const {
  try {
    this->x.saveBin(file);
  } catch (const std::exception &e) {
    Q_THROW(this, "Error writing range X:\n%s", e.what());
  }
  try {
    this->y.saveBin(file);
  } catch (const std::exception &e) {
    Q_THROW(this, "Error writing range Y:\n%s", e.what());
  }
  try {
    this->z.saveBin(file);
  } catch (const std::exception &e) {
    Q_THROW(this, "Error writing range Z:\n%s", e.what());
  }
  try {
    this->w.saveBin(file);
  } catch (const std::exception &e) {
    Q_THROW(this, "Error writing range W:\n%s", e.what());
  }
}

void RangeIter::saveBin(FILE *file) const {
  try {
    BinSerialization::writeValue<RangeIter::DATA_TYPE>(file, min, "min");
    BinSerialization::writeValue<RangeIter::DATA_TYPE>(file, max, "max");
  } catch (const std::exception &e) {
    Q_THROW(this, "Error writing BIN:\n%s", e.what());
  }
}
