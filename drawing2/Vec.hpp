/*
* File:   Vec.hpp
* Author: darek
*
* Created on 20 November 2014, 18:51
*/
#ifndef QFRACTAL_VEC_HPP
#define QFRACTAL_VEC_HPP

#include "ISerializable.hpp"
#include "utils.hpp"

enum Vec3Ind {
  Vec3IndX = 0,
  Vec3IndY = 1,
  Vec3IndZ = 2,
  Vec3IndCount = 3
};

#define VEC3F_BIN_SIZE (Vec3IndCount * sizeof(float))

union ValueVec3f {
  float v[Vec3IndCount];
  struct {
    float x;
    float y;
    float z;
  };
};

/** 3D float vector
*/
class Vec3f : public ISerializable {
  ValueVec3f  m_val;

public:
  virtual void readJson(const nlohmann::json &json);
  virtual nlohmann::json saveJson() const;
  virtual void readBin(FILE *file);
  virtual void saveBin(FILE *file) const;

  float  operator [](unsigned int i) const { return m_val.v[i]; }
  float& operator [](unsigned int i)       { return m_val.v[i]; }

  bool   operator ==(const Vec3f &q) const;
  bool   operator !=(const Vec3f &q) const { return !operator ==(q); }

  float&       x()            { return m_val.x; }
  float&       y()            { return m_val.y; }
  float&       z()            { return m_val.z; }

  const float& xConst() const { return m_val.x; }
  const float& yConst() const { return m_val.y; }
  const float& zConst() const { return m_val.z; }

  float*       data() { return m_val.v; }
  const float* dataConst() const { return m_val.v; }
  ValueVec3f&  value() { return m_val; }
  const ValueVec3f&  valueConst() const { return m_val; }

  void set(float x, float y, float z);
  void set(float xyz);
  void set(const Vec3f &other);

  void setZero();

  void add(const Vec3f &a);
  void add(const Vec3f &a, const Vec3f &b);

  void sub(const Vec3f &a);
  void sub(const Vec3f &a, const Vec3f &b);

  void mul(float s);

  void dot(const Vec3f &a);
  void dot(const Vec3f &a, const Vec3f &b);

  void cross(const Vec3f &a, const Vec3f &b);

  void scalarFunc(float(*func)(float));
  void scalarFunc(const Vec3f &a, float(*func)(float));

  float lengthSqr() const;

  void norm();
};

#endif
