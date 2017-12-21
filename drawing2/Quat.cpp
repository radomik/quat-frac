#include "stdafx.h"
#include "Quat.hpp"
#include "Vec.hpp"
#include "GeomMath.hpp"
#include "BinSerialization.hpp"

static const char *TAG_X = "x";
static const char *TAG_Y = "y";
static const char *TAG_Z = "z";
static const char *TAG_W = "w";

const char* Quatf::NAMES[] = {
  TAG_X, TAG_Y, TAG_Z, TAG_W
};

bool Quatf::operator ==(const Quatf &q) const {
  return vec4::equals(*this, q);
}

void Quatf::set(float x, float y, float z, float w) {
  vec4::set(*this, x, y, z, w);
}

void Quatf::set(float xyzw) {
  vec4::set(*this, xyzw);
}

void Quatf::set(const Quatf &other) {
  vec4::set(*this, other);
}

void Quatf::setZero() {
  vec4::setZero(*this);
}

void Quatf::add(const Quatf &other) {
  vec4::add(*this, other);
}

void Quatf::add(const Quatf &a, const Quatf &b) {
  vec4::add(*this, a, b);
}

void Quatf::sub(const Quatf &other) {
  vec4::sub(*this, other);
}

void Quatf::sub(const Quatf &a, const Quatf &b) {
  vec4::sub(*this, a, b);
}

void Quatf::mul(float s) {
  vec4::mul(*this, s);
}

void Quatf::mul_quat(const Quatf &a, const Quatf &b) {
  vec4::mul_quat(*this, a, b);
}

void Quatf::sqr_quat(const Quatf &s) {
  vec4::sqr_quat(*this, s);
}

void Quatf::dot(const Quatf &other) {
  vec4::dot(*this, other);
}

void Quatf::dot(const Quatf &a, const Quatf &b) {
  vec4::dot(*this, a, b);
}

void Quatf::scalarFunc(float(*func)(float)) {
  vec4::scalarFunc(*this, func);
}

void Quatf::scalarFunc(const Quatf &a, float(*func)(float)) {
  vec4::scalarFunc(*this, a, func);
}

float Quatf::lengthSqr() const {
  return vec4::lengthSqr(*this);
}

void Quatf::norm() {
  vec4::norm(*this);
}

void Quatf::print(FILE *file, const char *prefix, bool newline) const {
  fprintf(file, "%s[x=%.4f, y=%.4f, z=%.4f, w=%.4f]", (prefix) ? prefix : "",
    xConst(), yConst(), zConst(), wConst());
  if (newline) fputc('\n', file);
}

void Quatf::validate(const char *tag) const {
  // quaternion is always valid
}

void Quatf::print2(FILE *file, const char *prefix, bool newline) const {
  fprintf(file, "%s  %f  %f  %f  %f", (prefix) ? prefix : "",
    xConst(), yConst(), zConst(), wConst());
  if (newline) fputc('\n', file);
}

void Quatf::readJson(const nlohmann::json &json, const char *tag) {
  try {
    m_val.x = json[TAG_X];
    m_val.y = json[TAG_Y];
    m_val.z = json[TAG_Z];
    m_val.w = json[TAG_W];
  } catch (const std::exception &e) {
    Q_THROW(this, "Error reading JSON '%s':\n%s", tag, e.what());
  }
  this->validate(tag);
}

nlohmann::json Quatf::saveJson(const char *tag) const {
  try {
    return nlohmann::json({
        { TAG_X, m_val.x },
        { TAG_Y, m_val.y },
        { TAG_Z, m_val.z },
        { TAG_W, m_val.w }
    });
  } catch (const std::exception &e) {
    Q_THROW(this, "Error creating JSON '%s':\n%s", tag, e.what());
  }
}

void Quatf::readBin(FILE *file, const char *tag) {
  try {
    BinSerialization::readArray(file, m_val.v, sizeof(m_val.v) / sizeof(m_val.v[0]), tag);
  } catch (const std::exception &e) {
    Q_THROW(this, "Error reading BIN '%s':\n%s", tag, e.what());
  }
  this->validate(tag);
}

void Quatf::saveBin(FILE *file, const char *tag) const {
  try {
    BinSerialization::writeArray(file, m_val.v, sizeof(m_val.v) / sizeof(m_val.v[0]), tag);
  } catch (const std::exception &e) {
    Q_THROW(this, "Error writing BIN '%s':\n%s", tag, e.what());
  }
}
