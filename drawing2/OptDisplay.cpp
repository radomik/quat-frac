#include "stdafx.h"
#include "OptDisplay.hpp"
#include "BinSerialization.hpp"
#include "gfx.hpp"

void OptDisplayGeom::setDefault() {
  this->x = 0;
  this->y = 0;
  this->w = 800;
  this->h = 800;
}

void OptDisplayGeom::validate(const char *tag) const {
  try {
    if (this->w == 0 || this->h == 0) {
      Q_THROW(this, "Width 'w' and height 'h' must be above zero. Obtained w=%u, h=%u", this->w, this->h);
    }
  } catch (const std::exception &e) {
    Q_THROW(this, "Fail to validate '%s':\n%s", tag, e.what());
  }
}

void OptDisplay::setDefault() {
  this->background.r() = 1.0f;
  this->background.g() = 1.0f;
  this->background.b() = 1.0f;
  this->background.a() = 0.75f;
  this->disp_geom.setDefault();
}

void OptDisplay::validate(const char *tag) const {
  try {
    this->background.validate("background");
    this->disp_geom.validate("disp_geom");
  } catch (const std::exception &e) {
    Q_THROW(this, "Fail to validate '%s':\n%s", tag, e.what());
  }
}

void OptDisplayGeom::readJson(const nlohmann::json &json, const char *tag) {
  try {
    this->x = json["x"];
    this->y = json["y"];
    this->w = json["w"];
    this->h = json["h"];
  } catch (const std::exception &e) {
    Q_THROW(this, "Error reading JSON '%s':\n%s", tag, e.what());
  }
  this->validate(tag);
}

void OptDisplay::readJson(const nlohmann::json &json, const char *tag) {
  try {
    this->background.readJson(json["background"], "background");
    this->disp_geom.readJson(json["disp_geom"], "disp_geom");
  } catch (const std::exception &e) {
    Q_THROW(this, "Error reading JSON '%s':\n%s", tag, e.what());
  }
  this->validate(tag);
}

nlohmann::json OptDisplayGeom::saveJson(const char *tag) const {
  try {
    return nlohmann::json({
      { "x", this->x },
      { "y", this->y },
      { "w", this->w },
      { "h", this->h }
    });
  } catch (const std::exception &e) {
    Q_THROW(this, "Error creating JSON '%s':\n%s", tag, e.what());
  }
}

nlohmann::json OptDisplay::saveJson(const char *tag) const {
  try {
    return nlohmann::json({
      { "background", this->background.saveJson("background") },
      { "disp_geom", this->disp_geom.saveJson("disp_geom") }
    });
  } catch (const std::exception &e) {
    Q_THROW(this, "Error creating JSON '%s':\n%s", tag, e.what());
  }
}

void OptDisplayGeom::readBin(FILE *file, const char *tag) {
  try {
    BinSerialization::readValue(file, this->x, "x");
    BinSerialization::readValue(file, this->y, "y");
    BinSerialization::readValue(file, this->w, "w");
    BinSerialization::readValue(file, this->h, "h");
  } catch (const std::exception &e) {
    Q_THROW(this, "Error reading BIN '%s':\n%s", tag, e.what());
  }
  this->validate(tag);
}

void OptDisplay::readBin(FILE *file, const char *tag) {
  try {
    this->background.readBin(file, "background");
    this->disp_geom.readBin(file, "disp_geom");
  } catch (const std::exception &e) {
    Q_THROW(this, "Error reading BIN '%s':\n%s", tag, e.what());
  }
  this->validate(tag);
}

void OptDisplay::saveBin(FILE *file, const char *tag) const {
  try {
    this->background.saveBin(file, "background");
    this->disp_geom.saveBin(file, "disp_geom");
  } catch (const std::exception &e) {
    Q_THROW(this, "Error writing BIN '%s':\n%s", tag, e.what());
  }
}

void OptDisplayGeom::saveBin(FILE *file, const char *tag) const {
  try {
    BinSerialization::writeValue(file, this->x, "x");
    BinSerialization::writeValue(file, this->y, "y");
    BinSerialization::writeValue(file, this->w, "w");
    BinSerialization::writeValue(file, this->h, "h");
  } catch (const std::exception &e) {
    Q_THROW(this, "Error writing BIN '%s':\n%s", tag, e.what());
  }
}

void OptDisplay::_setBackground() {
  glClearColor(this->background.rConst(), this->background.gConst(), this->background.bConst(), this->background.aConst());
}
