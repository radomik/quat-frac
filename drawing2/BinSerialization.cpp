#include "BinSerialization.hpp"

const unsigned char BinSerialization::BOOLEAN_TRUE = 0xFF;
const unsigned char BinSerialization::BOOLEAN_FALSE = 0x00;

template<typename T>
void BinSerialization::writeValue(FILE *file, const T &value, const char *tag) {
  size_t item_size = sizeof(T);
  size_t retv = fwrite(&value, item_size, 1, file);
  if (retv != 1) {
    SysError ex;
    Q_STTHROW("Error writing '%s' of size %zu B [retv=%zu, errno=%s]", tag, item_size, retv, ex.msg);
  }
}

template<typename T>
void BinSerialization::readValue(FILE *file, T &value, const char *tag) {
  size_t item_size = sizeof(T);
  size_t retv = fread(&value, item_size, 1, file);
  if (retv != 1) {
    SysError ex;
    Q_STTHROW("Error reading '%s' of size %zu B [retv=%zu, errno=%s]", tag, item_size, retv, ex.msg);
  }
}

template<typename T>
void BinSerialization::writeValueAsOneByte(FILE *file, const T &value, const char *tag) {
  const unsigned char v = (unsigned char)value;
  BinSerialization::writeValue(file, v, tag);
}

template<typename T>
void BinSerialization::readValueAsOneByte(FILE *file, T &value, const char *tag) {
  unsigned char v;
  BinSerialization::readValue<unsigned char>(file, v, tag);
  value = (T)v;
}

template<typename T>
void BinSerialization::writeArray(FILE *file, const T *array, size_t item_count, const char *tag) {
  size_t item_size = sizeof(T);
  size_t retv = fwrite(array, item_size, item_count, file);
  if (retv != item_count) {
    SysError ex;
    Q_STTHROW("Error writing array '%s' of %zu items, %zu B each [retv=%zu, errno=%s]",
      tag, item_count, item_size, retv, ex.msg);
  }
}

template<typename T>
void BinSerialization::readArray(FILE *file, T *array, size_t item_count, const char *tag) {
  size_t item_size = sizeof(T);
  size_t retv = fread(array, item_size, item_count, file);
  if (retv != item_count) {
    SysError ex;
    Q_STTHROW("Error reading array '%s' of %zu items, %zu B each [retv=%zu, errno=%s]",
      tag, item_count, item_size, retv, ex.msg);
  }
}

void BinSerialization::writeBool(FILE *file, const bool &value, const char *tag) {
  const unsigned char v = (value
    ? BinSerialization::BOOLEAN_TRUE
    : BinSerialization::BOOLEAN_FALSE);
  BinSerialization::writeValue<unsigned char>(file, v, tag);
}

void BinSerialization::readBool(FILE *file, bool &value, const char *tag) {
  unsigned char v;
  BinSerialization::readValue<unsigned char>(file, v, tag);
  if (v == BinSerialization::BOOLEAN_TRUE) { value = true; return; }
  if (v == BinSerialization::BOOLEAN_FALSE) { value = false; return; }
  Q_STTHROW("Invalid boolean value 0x%02X (expected 0x%02X or 0x%02X)",
    v, BinSerialization::BOOLEAN_TRUE, BinSerialization::BOOLEAN_FALSE);
}
