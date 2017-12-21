/*
* File:   Quat.h
* Author: darek
*
* Created on 16 June 2014
*/
#ifndef QFRACTAL_QUAT_H
#define QFRACTAL_QUAT_H

#include "FloatingPoint.hpp"
#include "Vec.hpp"
#include "utils.hpp"

enum QuatInd {
  QuatIndX = 0,
  QuatIndY = 1,
  QuatIndZ = 2,
  QuatIndW = 3,
  QuatIndCount = 4
};

#define QUATF_BIN_SIZE (QuatIndCount * sizeof(float))

union ValueQuatf {
#ifdef _SSE
  __m128  m;
#endif
  float v[QuatIndCount];
  struct {
    float x;
    float y;
    float z;
    float w;
  };
};

/** Quaternion class
* q = w + x*i + y*j + z*k
*/
class Quatf : public ISerializable {
  ValueQuatf  m_val;
public:
  static const char *NAMES[];

  virtual void readJson(const nlohmann::json &json);
  virtual nlohmann::json saveJson() const;
  virtual void readBin(FILE *file);
  virtual void saveBin(FILE *file) const;

  float  operator [](unsigned int i) const { return m_val.v[i]; }
  float& operator [](unsigned int i)       { return m_val.v[i]; }
  bool   operator ==(const Quatf &q) const;
  bool   operator !=(const Quatf &q) const { return !operator ==(q); }

  float*       data() { return m_val.v; }
  const float* dataConst() const { return m_val.v; }
  ValueQuatf&  value() { return m_val; }
  const ValueQuatf&  valueConst() const { return m_val; }

  float&       x()            { return m_val.x; }
  float&       y()            { return m_val.y; }
  float&       z()            { return m_val.z; }
  float&       w()            { return m_val.w; }

  const float& xConst() const { return m_val.x; }
  const float& yConst() const { return m_val.y; }
  const float& zConst() const { return m_val.z; }
  const float& wConst() const { return m_val.w; }

  void set(float x, float y, float z, float w);
  void set(float xyzw);
  void set(const Quatf &other);
  void setZero();

  void add(const Quatf &other);
  void add(const Quatf &a, const Quatf &b);

  void sub(const Quatf &other);
  void sub(const Quatf &a, const Quatf &b);

  void mul(const Quatf &a, const Quatf &b);
  void mul(float s);

  void mul_quat(const Quatf &a, const Quatf &b);
  void sqr_quat(const Quatf &s);

  void dot(const Quatf &other);
  void dot(const Quatf &a, const Quatf &b);

  void scalarFunc(float(*func)(float));
  void scalarFunc(const Quatf &a, float(*func)(float));

  float lengthSqr() const;

  void norm();

  void print(FILE *file, const char *prefix = "", bool newline = false) const;
  void print2(FILE *file, const char *prefix = "", bool newline = false) const;
};

#endif
