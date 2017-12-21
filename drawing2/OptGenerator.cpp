#include "stdafx.h"
#include "OptGenerator.hpp"
#include "BinSerialization.hpp"

const char* OptGenerator::TAG_BASE_PATH = "opt_generator";
const char* OptGenerator::TAG_VERSION = "version";
const char* OptGenerator::TAG_BUF_SIZE = "buf_size";
const char* OptGenerator::TAG_MUL_RANGE_BY_PI = "mul_range_by_pi";
const char* OptGenerator::TAG_INITIAL = "initial";
const char* OptGenerator::TAG_MAX_LEN_SQR = "max_len_sqr";
const char* OptGenerator::TAG_FUN_INDEX = "fun_index";
const char* OptGenerator::TAG_ITER = "iter";
const char* OptGenerator::TAG_RANGE = "range";
const char* OptGenerator::TAG_MOD_Q = "mod_q";
const size_t OptGenerator::MAX_BUF_SIZE = 10*1024*1024;

void OptGenerator::validate() const {
  this->version != 1 && this->version != 2) {
    Q_THROW(this, "Invalid version=%u (expected {1,2})", this->version);
  }
  if (this->buf_size > OptGenerator::MAX_BUF_SIZE) {
    Q_THROW(this, "Invalid buf_size=%u (expected at most %zu bytes)",
      this->buf_size, OptGenerator::MAX_BUF_SIZE);
  }
}

void OptGenerator::readJson(const nlohmann::json &json, const char *tag)
{
  try {
    this->version = json[TAG_VERSION];
    this->buf_size = json[TAG_BUF_SIZE];
    this->mul_range_by_pi = json[TAG_MUL_RANGE_BY_PI];
    this->initial.readJson(json[TAG_INITIAL], TAG_INITIAL);
    this->max_len_sqr = json[TAG_MAX_LEN_SQR];
    int fun_index_value = json[TAG_FUN_INDEX];
    FractalFuncInd fi;
    fi.parse(fun_index_value);
    this->fun_index = fi.value;
    this->iter.readJson(json[TAG_ITER], TAG_ITER);
    this->range.readJson(json[TAG_RANGE], TAG_RANGE);
    this->mod_q = json[TAG_MOD_Q];
  } catch (const std::exception &e) {
    Q_THROW(this, "Error reading JSON '%s':\n%s", tag, e.what());
  }
  try {
    this->validate();
  } catch (const std::exception &e) {
    Q_THROW(this, "Fail to validate '%s':\n%s", tag, e.what());
  }
}

nlohmann::json OptGenerator::saveJson(const char *tag) const
{
  try {
    return nlohmann::json({
        { TAG_VERSION, this->version },
        { TAG_BUF_SIZE, this->buf_size },
        { TAG_MUL_RANGE_BY_PI, this->mul_range_by_pi },
        { TAG_INITIAL, this->initial.saveJson(TAG_INITIAL) },
        { TAG_MAX_LEN_SQR, this->max_len_sqr },
        { TAG_FUN_INDEX, this->fun_index },
        { TAG_ITER, this->iter.saveJson(TAG_ITER) },
        { TAG_RANGE, this->range.saveJson(TAG_RANGE) },
        { TAG_MOD_Q, this->mod_q }
    });
  } catch (const std::exception &e) {
    Q_THROW(this, "Error creating JSON '%s':\n%s", tag, e.what());
  }
}

void OptGenerator::readBin(FILE *file, const char *tag)
{
  try {
    BinSerialization::readValue(file, this->version, TAG_VERSION);
    BinSerialization::readValue(file, this->buf_size, TAG_BUF_SIZE);
    BinSerialization::readBool(file, this->mul_range_by_pi, TAG_MUL_RANGE_BY_PI);
    this->initial.readBin(file, TAG_INITIAL);
    BinSerialization::readValue(file, this->max_len_sqr, TAG_MAX_LEN_SQR);
    BinSerialization::readValueAsOneByte(file, this->fun_index, TAG_FUN_INDEX);
    this->iter.readBin(file, TAG_ITER);
    this->range.readBin(file, TAG_RANGE);
    BinSerialization::readValue(file, this->mod_q, TAG_MOD_Q);
  } catch (const std::exception &e) {
    Q_THROW(this, "Error reading BIN '%s':\n%s", tag, e.what());
  }
  try {
    this->validate();
  } catch (const std::exception &e) {
    Q_THROW(this, "Fail to validate '%s':\n%s", tag, e.what());
  }
}

void OptGenerator::saveBin(FILE *file, const char *tag) const
{
  try {
    BinSerialization::writeValueAsOneByte(file, this->version, TAG_VERSION);
    BinSerialization::writeValue(file, this->buf_size, TAG_BUF_SIZE);
    BinSerialization::writeBool(file, this->mul_range_by_pi, TAG_MUL_RANGE_BY_PI);
    this->initial.saveBin(file, TAG_INITIAL);
    BinSerialization::writeValue(file, this->max_len_sqr, TAG_MAX_LEN_SQR);
    BinSerialization::writeValueAsOneByte(file, this->fun_index, TAG_FUN_INDEX);
    this->iter.saveBin(file, TAG_ITER);
    this->range.saveBin(file, TAG_RANGE);
    BinSerialization::writeValue(file, this->mod_q, TAG_MOD_Q);
  } catch (const std::exception &e) {
    Q_THROW(this, "Error writing BIN '%s':\n%s", tag, e.what());
  }
}
