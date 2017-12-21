#include "stdafx.h"
#include "Quat.hpp"
#include "Vec.hpp"
#include "GeomMath.hpp"
#include "BinSerialization.hpp"

const char* Quatf::NAMES[] = {
  "x", "y", "z", "w"
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
  if (newline) putchar('\n');
}

void Quatf::print2(FILE *file, const char *prefix, bool newline) const {
  fprintf(file, "%s  %f  %f  %f  %f", (prefix) ? prefix : "",
    xConst(), yConst(), zConst(), wConst());
  if (newline) putchar('\n');
}

int Quatf::readJson(const nlohmann::json &json) {
  m_val.x = json["x"];
  m_val.y = json["y"];
  m_val.z = json["z"];
  m_val.w = json["w"];
  return 0;
}

nlohmann::json Quatf::saveJson() const {
  return nlohmann::json({
      { "x", m_val.x },
      { "y", m_val.y },
      { "z", m_val.z },
      { "w", m_val.w }
  });
}

int Quatf::readBin(FILE *file) {
  return BinSerialization::readArray<float>(file, m_val.v, sizeof(m_val.v) / sizeof(m_val.v[0]));
}

int Quatf::saveBin(FILE *file) const {
  return BinSerialization::writeArray<float>(file, m_val.v, sizeof(m_val.v) / sizeof(m_val.v[0]));
}
