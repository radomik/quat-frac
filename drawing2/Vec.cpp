#include "stdafx.h"
#include "Vec.hpp"
#include "FloatingPoint.hpp"
#include "GeomMath.hpp"
#include "BinSerialization.hpp"

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

int Vec3f::readJson(const nlohmann::json &json) {
  m_val.x = json["x"];
  m_val.y = json["y"];
  m_val.z = json["z"];
  return 0;
}

nlohmann::json Vec3f::saveJson() const {
  return nlohmann::json({
      { "x", m_val.x },
      { "y", m_val.y },
      { "z", m_val.z }
  });
}

int Vec3f::readBin(FILE *file) {
  return BinSerialization::readArray<float>(file, m_val.v, sizeof(m_val.v)/sizeof(m_val.v[0]));
}

int Vec3f::saveBin(FILE *file) const {
  return BinSerialization::writeArray<float>(file, m_val.v, sizeof(m_val.v)/sizeof(m_val.v[0]));
}
