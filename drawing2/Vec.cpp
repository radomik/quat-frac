#include "stdafx.h"
#include "Vec.hpp"
#include "FloatingPoint.hpp"
#include "GeomMath.hpp"
#include "BinSerialization.hpp"

static const char *TAG_X = "x";
static const char *TAG_Y = "y";
static const char *TAG_Z = "z";

bool Vec3f::operator ==(const Vec3f &q) const {
  return vec3::equals(*this, q);
}

void Vec3f::set(float x, float y, float z) {
  vec3::set(*this, x, y, z);
}

void Vec3f::set(float xyz) {
  vec3::set(*this, xyz);
}

void Vec3f::set(const Vec3f &other) {
  vec3::set(*this, other);
}

void Vec3f::setZero() {
  vec3::setZero(*this);
}

void Vec3f::add(const Vec3f &a) {
  vec3::add(*this, a);
}

void Vec3f::add(const Vec3f &a, const Vec3f &b) {
  vec3::add(*this, a, b);
}

void Vec3f::sub(const Vec3f &a) {
  vec3::sub(*this, a);
}

void Vec3f::sub(const Vec3f &a, const Vec3f &b) {
  vec3::sub(*this, a, b);
}

void Vec3f::mul(float s) {
  vec3::mul(*this, s);
}

void Vec3f::dot(const Vec3f &a) {
  vec3::dot(*this, a);
}

void Vec3f::dot(const Vec3f &a, const Vec3f &b) {
  vec3::dot(*this, a, b);
}

void Vec3f::cross(const Vec3f &a, const Vec3f &b) {
  vec3::cross(*this, a, b);
}

void Vec3f::scalarFunc(float (*func)(float)) {
  vec3::scalarFunc(*this, func);
}

void Vec3f::scalarFunc(const Vec3f &a, float (*func)(float)) {
  vec3::scalarFunc(*this, a, func);
}

float Vec3f::lengthSqr() const {
  return vec3::lengthSqr(*this);
}

void Vec3f::norm() {
  vec3::norm(*this);
}

void Vec3f::print(FILE *file, const char *prefix, bool newline) const {
  fprintf(file, "%s[x=%.4f, y=%.4f, z=%.4f]", (prefix) ? prefix : "",
    xConst(), yConst(), zConst());
  if (newline) fputc('\n', file);
}

void Vec3f::print2(FILE *file, const char *prefix, bool newline) const {
  fprintf(file, "%s  %f  %f  %f", (prefix) ? prefix : "",
    xConst(), yConst(), zConst());
  if (newline) fputc('\n', file);
}

void Vec3f::validate(const char *tag) const {
  // vector is always valid
}

void Vec3f::readJson(const nlohmann::json &json, const char *tag) {
  try {
    m_val.x = json[TAG_X];
    m_val.y = json[TAG_Y];
    m_val.z = json[TAG_Z];
  } catch (const std::exception &e) {
    Q_THROW(this, "Error reading JSON '%s':\n%s", tag, e.what());
  }
  this->validate(tag);
}

nlohmann::json Vec3f::saveJson(const char *tag) const {
  try {
    return nlohmann::json({
        { TAG_X, m_val.x },
        { TAG_Y, m_val.y },
        { TAG_Z, m_val.z }
    });
  } catch (const std::exception &e) {
    Q_THROW(this, "Error creating JSON '%s':\n%s", tag, e.what());
  }
}

void Vec3f::readBin(FILE *file, const char *tag) {
  try {
    BinSerialization::readArray(file, m_val.v, sizeof(m_val.v)/sizeof(m_val.v[0]), tag);
  } catch (const std::exception &e) {
    Q_THROW(this, "Error reading BIN '%s':\n%s", tag, e.what());
  }
}

void Vec3f::saveBin(FILE *file, const char *tag) const {
  try {
    BinSerialization::writeArray(file, m_val.v, sizeof(m_val.v)/sizeof(m_val.v[0]), tag);
  } catch (const std::exception &e) {
    Q_THROW(this, "Error writing BIN '%s':\n%s", tag, e.what());
  }
}
