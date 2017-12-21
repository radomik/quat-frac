/*
* File:   GeomMath.hpp
* Author: darek
*
* Created on 26 December 2014, 20:57
*/
#ifndef QFRACTAL_GEOMMATH_HPP
#define QFRACTAL_GEOMMATH_HPP

#include "utils.hpp"
#include "FloatingPoint.hpp"
#include "Vec.hpp"
#include "Quat.hpp"
#include <math.h>

namespace vec3 {
  inline void set(float *res, float x, float y, float z) {
    res[0] = x; res[1] = y; res[2] = z;
  }

  inline void set(ValueVec3f &res, float x, float y, float z) {
    set(res.v, x, y, z);
  }

  inline void set(Vec3f &res, float x, float y, float z) {
    set(res.value(), x, y, z);
  }

  inline void set(float *res, float value) {
    for (unsigned int i = 0; i < 3; i++) res[i] = value;
  }

  inline void set(ValueVec3f &res, float value) {
    set(res.v, value);
  }

  inline void set(Vec3f &res, float value) {
    set(res.value(), value);
  }

  inline void set(float *res, const float *src) {
    memcpy(res, src, 3 * sizeof(float));
  }

  inline void set(ValueVec3f &res, const ValueVec3f &src) {
    set(res.v, src.v);
  }

  inline void set(Vec3f &res, const Vec3f &src) {
    set(res.value(), src.valueConst());
  }

  inline void setZero(float *res) {
    set(res, 0.0f);
  }

  inline void setZero(ValueVec3f &res) {
    setZero(res.v);
  }

  inline void setZero(Vec3f &res) {
    setZero(res.value());
  }

  inline void add(float *res, const float *a) {
    for (unsigned int i = 0; i < 3; i++) res[i] += a[i];
  }

  inline void add(ValueVec3f &res, const ValueVec3f &a) {
    add(res.v, a.v);
  }

  inline void add(Vec3f &res, const Vec3f &a) {
    add(res.value(), a.valueConst());
  }

  inline void add(float *res, const float *a, const float *b) {
    for (unsigned int i = 0; i < 3; i++) res[i] = a[i] + b[i];
  }

  inline void add(ValueVec3f &res, const ValueVec3f &a, const ValueVec3f &b) {
    add(res.v, a.v, b.v);
  }

  inline void add(Vec3f &res, const Vec3f &a, const Vec3f &b) {
    add(res.value(), a.valueConst(), b.valueConst());
  }

  inline void sub(float *res, const float *a) {
    for (unsigned int i = 0; i < 3; i++) res[i] -= a[i];
  }

  inline void sub(ValueVec3f &res, const ValueVec3f &a) {
    sub(res.v, a.v);
  }

  inline void sub(Vec3f &res, const Vec3f &a) {
    sub(res.value(), a.valueConst());
  }

  inline void sub(float *res, const float *a, const float *b) {
    for (unsigned int i = 0; i < 3; i++) res[i] = a[i] - b[i];
  }

  inline void sub(ValueVec3f &res, const ValueVec3f &a, const ValueVec3f &b) {
    sub(res.v, a.v, b.v);
  }

  inline void sub(Vec3f &res, const Vec3f &a, const Vec3f &b) {
    sub(res.value(), a.valueConst(), b.valueConst());
  }

  inline void mul(float *res, float s) {
    for (unsigned int i = 0; i < 3; i++) res[i] *= s;
  }

  inline void mul(ValueVec3f &res, float s) {
    mul(res.v, s);
  }

  inline void mul(Vec3f &res, float s) {
    mul(res.value(), s);
  }

  inline void dot(float *res, const float *a) {
    for (unsigned int i = 0; i < 3; i++) res[i] *= a[i];
  }

  inline void dot(ValueVec3f &res, const ValueVec3f &a) {
    dot(res.v, a.v);
  }

  inline void dot(Vec3f &res, const Vec3f &a) {
    dot(res.value(), a.valueConst());
  }

  inline void dot(float *res, const float *a, const float *b) {
    for (unsigned int i = 0; i < 3; i++) res[i] = a[i] * b[i];
  }

  inline void dot(ValueVec3f &res, const ValueVec3f &a, const ValueVec3f &b) {
    dot(res.v, a.v, b.v);
  }

  inline void dot(Vec3f &res, const Vec3f &a, const Vec3f &b) {
    dot(res.value(), a.valueConst(), b.valueConst());
  }

  inline void dot_aa(float *res, const float *a) {
    for (unsigned int i = 0; i < 3; i++) res[i] = a[i] * a[i];
  }

  inline void dot_aa(ValueVec3f &res, const ValueVec3f &a) {
    dot_aa(res.v, a.v);
  }

  inline void dot_aa(Vec3f &res, const Vec3f &a) {
    dot_aa(res.value(), a.valueConst());
  }

  inline void cross(float *res, const float *a, const float *b) {
    res[0] = a[1] * b[2] - a[2] * b[1];
    res[1] = a[2] * b[0] - a[0] * b[2];
    res[2] = a[0] * b[1] - a[1] * b[0];
  }

  inline void cross(ValueVec3f &res, const ValueVec3f &a, const ValueVec3f &b) {
    cross(res.v, a.v, b.v);
  }

  inline void cross(Vec3f &res, const Vec3f &a, const Vec3f &b) {
    cross(res.value(), a.valueConst(), b.valueConst());
  }

  inline void scalarFunc(float *res, float(*func)(float)) {
    for (unsigned int i = 0; i < 3; i++) res[i] = func(res[i]);
  }

  inline void scalarFunc(ValueVec3f &res, float(*func)(float)) {
    scalarFunc(res.v, func);
  }

  inline void scalarFunc(Vec3f &res, float(*func)(float)) {
    scalarFunc(res.value(), func);
  }

  inline void scalarFunc(float *res, const float *a, float(*func)(float)) {
    for (unsigned int i = 0; i < 3; i++) res[i] = func(a[i]);
  }

  inline void scalarFunc(ValueVec3f &res, const ValueVec3f &a, float(*func)(float)) {
    scalarFunc(res.v, a.v, func);
  }

  inline void scalarFunc(Vec3f &res, const Vec3f &a, float(*func)(float)) {
    scalarFunc(res.value(), a.valueConst(), func);
  }

  inline float lengthSqr(const float *a) {
    float res = 0.0f;
    for (unsigned int i = 0; i < 3; i++) res += a[i] * a[i];
    return res;
  }

  inline float lengthSqr(const ValueVec3f &a) {
    return lengthSqr(a.v);
  }

  inline float lengthSqr(const Vec3f &a) {
    return lengthSqr(a.valueConst());
  }

  inline void norm(float *res) {
    float len = lengthSqr(res);
    if (!Float::cmp(len, 0.0f)) return;
    len = sqrtf(len);
    for (unsigned int i = 0; i < 3; i++) res[i] /= len;
  }

  inline void norm(ValueVec3f &res) {
    norm(res.v);
  }

  inline void norm(Vec3f &res) {
    norm(res.value());
  }

  inline bool equals(const float *a, const float *b) {
    for (unsigned int i = 0; i < 3; i++)
      if (!Float::equ(a[i], b[i])) return false;

    return true;
  }

  inline bool equals(const ValueVec3f &a, const ValueVec3f &b) {
    return equals(a.v, b.v);
  }

  inline bool equals(const Vec3f &a, const Vec3f &b) {
    return equals(a.valueConst(), b.valueConst());
  }
};

namespace vec4 {
  inline void set(float *res, float x, float y, float z, float w) {
    res[0] = x; res[1] = y; res[2] = z; res[3] = w;
  }

  inline void set(ValueQuatf &res, float x, float y, float z, float w) {
    set(res.v, x, y, z, w);
  }

  inline void set(Quatf &res, float x, float y, float z, float w) {
    set(res.value(), x, y, z, w);
  }

  inline void set(float *res, float value) {
    for (unsigned int i = 0; i < 4; i++) res[i] = value;
  }

  inline void set(ValueQuatf &res, float value) {
    set(res.v, value);
  }

  inline void set(Quatf &res, float value) {
    set(res.value(), value);
  }

  inline void set(float *res, const float *src) {
    memcpy(res, src, 4 * sizeof(float));
  }

  inline void set(ValueQuatf &res, const ValueQuatf &src) {
    set(res.v, src.v);
  }

  inline void set(Quatf &res, const Quatf &src) {
    set(res.value(), src.valueConst());
  }

  inline void setZero(float *res) {
    set(res, 0.0f);
  }

  inline void setZero(ValueQuatf &res) {
    setZero(res.v);
  }

  inline void setZero(Quatf &res) {
    setZero(res.value());
  }

  inline void add(float *res, const float *a) {
    for (unsigned int i = 0; i < 4; i++) res[i] += a[i];
  }

  inline void add(ValueQuatf &res, const ValueQuatf &a) {
    add(res.v, a.v);
  }

  inline void add(Quatf &res, const Quatf &a) {
    add(res.value(), a.valueConst());
  }

  inline void add(float *res, const float *a, const float *b) {
    for (unsigned int i = 0; i < 4; i++) res[i] = a[i] + b[i];
  }


  inline void add(ValueQuatf &res, const ValueQuatf &a, const ValueQuatf &b) {
    add(res.v, a.v, b.v);
  }

  inline void add(Quatf &res, const Quatf &a, const Quatf &b) {
    add(res.value(), a.valueConst(), b.valueConst());
  }

  inline void sub(float *res, const float *a) {
    for (unsigned int i = 0; i < 4; i++) res[i] -= a[i];
  }

  inline void sub(ValueQuatf &res, const ValueQuatf &a) {
    sub(res.v, a.v);
  }

  inline void sub(Quatf &res, const Quatf &a) {
    sub(res.value(), a.valueConst());
  }

  inline void sub(float *res, const float *a, const float *b) {
    for (unsigned int i = 0; i < 4; i++) res[i] = a[i] - b[i];
  }

  inline void sub(ValueQuatf &res, const ValueQuatf &a, const ValueQuatf &b) {
    sub(res.v, a.v, b.v);
  }

  inline void sub(Quatf &res, const Quatf &a, const Quatf &b) {
    sub(res.value(), a.valueConst(), b.valueConst());
  }

  inline void mul(float *res, float s) {
    for (unsigned int i = 0; i < 4; i++) res[i] *= s;
  }

  inline void mul(ValueQuatf &res, float s) {
    mul(res.v, s);
  }

  inline void mul(Quatf &res, float s) {
    mul(res.value(), s);
  }

  inline void dot(float *res, const float *a) {
    for (unsigned int i = 0; i < 4; i++) res[i] *= a[i];
  }

  inline void dot(ValueQuatf &res, const ValueQuatf &a) {
    dot(res.v, a.v);
  }

  inline void dot(Quatf &res, const Quatf &a) {
    dot(res.value(), a.valueConst());
  }

  inline void dot(float *res, const float *a, const float *b) {
    for (unsigned int i = 0; i < 4; i++) res[i] = a[i] * b[i];
  }

  inline void dot(ValueQuatf &res, const ValueQuatf &a, const ValueQuatf &b) {
    dot(res.v, a.v, b.v);
  }

  inline void dot(Quatf &res, const Quatf &a, const Quatf &b) {
    dot(res.value(), a.valueConst(), b.valueConst());
  }

  inline void dot_aa(float *res, const float *a) {
    for (unsigned int i = 0; i < 4; i++) res[i] = a[i] * a[i];
  }

  inline void dot_aa(ValueQuatf &res, const ValueQuatf &a) {
    dot_aa(res.v, a.v);
  }

  inline void dot_aa(Quatf &res, const Quatf &a) {
    dot_aa(res.value(), a.valueConst());
  }

  inline void mul_quat(float *res, const float *a, const float *b) {
    res[0] = a[3] * b[0] + a[0] * b[3] + a[1] * b[2] - a[2] * b[1];
    res[1] = a[3] * b[1] + a[1] * b[3] + a[2] * b[0] - a[0] * b[2];
    res[2] = a[3] * b[2] + a[2] * b[3] + a[0] * b[1] - a[1] * b[0];
    res[3] = a[3] * b[3] - a[0] * b[0] - a[1] * b[1] - a[2] * b[2];
  }

  inline void mul_quat(ValueQuatf &res, const ValueQuatf &a, const ValueQuatf &b) {
    mul_quat(res.v, a.v, b.v);
  }

  inline void mul_quat(Quatf &res, const Quatf &a, const Quatf &b) {
    mul_quat(res.value(), a.valueConst(), b.valueConst());
  }

  inline void sqr_quat(float *res, const float *s) {
    res[0] = 2.0f*s[3] * s[0];
    res[1] = 2.0f*s[3] * s[1];
    res[2] = 2.0f*s[3] * s[2];
    res[3] = s[3] * s[3] - s[0] * s[0] - s[1] * s[1] - s[2] * s[2];
  }

  inline void sqr_quat(ValueQuatf &res, const ValueQuatf &s) {
    sqr_quat(res.v, s.v);
  }

  inline void sqr_quat(Quatf &res, const Quatf &s) {
    sqr_quat(res.value(), s.valueConst());
  }

  inline void scalarFunc(float *res, float(*func)(float)) {
    for (unsigned int i = 0; i < 4; i++) res[i] = func(res[i]);
  }

  inline void scalarFunc(ValueQuatf &res, float(*func)(float)) {
    scalarFunc(res.v, func);
  }

  inline void scalarFunc(Quatf &res, float(*func)(float)) {
    scalarFunc(res.value(), func);
  }

  inline void scalarFunc(float *res, const float *a, float(*func)(float)) {
    for (unsigned int i = 0; i < 4; i++) res[i] = func(a[i]);
  }

  inline void scalarFunc(ValueQuatf &res, const ValueQuatf &a, float(*func)(float)) {
    scalarFunc(res.v, a.v, func);
  }

  inline void scalarFunc(Quatf &res, const Quatf &a, float(*func)(float)) {
    scalarFunc(res.value(), a.valueConst(), func);
  }

  inline float lengthSqr(const float *a) {
    float res = 0.0f;
    for (unsigned int i = 0; i < 4; i++) res += a[i] * a[i];
    return res;
  }

  inline float lengthSqr(const ValueQuatf &a) {
    return lengthSqr(a.v);
  }

  inline float lengthSqr(const Quatf &a) {
    return lengthSqr(a.valueConst());
  }

  inline void norm(float *res) {
    float len = lengthSqr(res);
    if (!Float::cmp(len, 0.0f)) return;
    len = sqrtf(len);
    for (unsigned int i = 0; i < 4; i++) res[i] /= len;
  }

  inline void norm(ValueQuatf &res) {
    norm(res.v);
  }

  inline void norm(Quatf &res) {
    norm(res.value());
  }

  inline bool equals(const float *a, const float *b) {
    for (unsigned int i = 0; i < 4; i++)
      if (!Float::equ(a[i], b[i])) return false;

    return true;
  }

  inline bool equals(const ValueQuatf &a, const ValueQuatf &b) {
    return equals(a.v, b.v);
  }

  inline bool equals(const Quatf &a, const Quatf &b) {
    return equals(a.valueConst(), b.valueConst());
  }
};

#endif

