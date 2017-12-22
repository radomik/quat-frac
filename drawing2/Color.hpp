/*
 * File:   Color.hpp
 * Author: darek
 *
 * Created on 23 November 2014, 23:18
 */
#ifndef QFRACTAL_DRAWING_COLOR_HPP
#define QFRACTAL_DRAWING_COLOR_HPP

#include "ISerializable.hpp"
#include "utils.hpp"

enum Color4Ind {
  Color4IndR = 0,
  Color4IndG = 1,
  Color4IndB = 2,
  Color4IndA = 3,
  Color4IndCount = 4
};

#define COLOR4_BIN_SIZE (Color4IndCount * sizeof(float))

union ValueColor4f {
  float v[Color4IndCount];
  struct {
    float r;
    float g;
    float b;
    float a;
  };

  ValueColor4f() { }

  ValueColor4f(float r, float g, float b, float a) {
    this->r = r; this->g = g; this->b = b; this->a = a;
  }
};

class Color4f : public ISerializable {
private:
  ValueColor4f m_val;

public:
  virtual void readJson(const nlohmann::json &json, const char *tag);
  virtual nlohmann::json saveJson(const char *tag) const;

  virtual void readBin(FILE *file, const char *tag);
  virtual void saveBin(FILE *file, const char *tag) const;

  virtual void validate(const char *tag) const;

  float  operator [](unsigned int i) const { return m_val.v[i]; }
  float& operator [](unsigned int i)       { return m_val.v[i]; }
  bool   operator ==(const Color4f& c) const;
  bool   operator !=(const Color4f& c) const { return !operator==(c); }

  float& r() { return m_val.r; }
  float& g() { return m_val.g; }
  float& b() { return m_val.b; }
  float& a() { return m_val.a; }

  const float& rConst() const { return m_val.r; }
  const float& gConst() const { return m_val.g; }
  const float& bConst() const { return m_val.b; }
  const float& aConst() const { return m_val.a; }

  float*       data() { return m_val.v; }
  const float* dataConst() const { return m_val.v; }

  void toArray(float *out_val) const {
    memcpy(out_val, m_val.v, sizeof(m_val.v));
  }

  void fromArray(const float *in_val) {
    memcpy(m_val.v, in_val, sizeof(m_val.v));
  }

  static bool validChannel(float value);

private:
  static int _invalChannel(const char* channel, float value);
};

#endif
