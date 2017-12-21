/*
* File:   BinSerialization.hpp
* Author: darek
*
* Created on 20 November 2014, 15:17
*/
#ifndef QFRACTAL_BINSERIALIZATION_HPP
#define QFRACTAL_BINSERIALIZATION_HPP

#include "utils.hpp"

class BinSerialization {
public:
  static const unsigned char BOOLEAN_TRUE;
  static const unsigned char BOOLEAN_FALSE;

  /** Save primitive value into file */
  template<typename T>
  static void writeValue(FILE *file, const T &value, const char *tag);

  /** Read primitive value from file */
  template<typename T>
  static void readValue(FILE *file, T &value, const char *tag);

  /** Save primitive value as one byte into file */
  template<typename T>
  static void writeValueAsOneByte(FILE *file, const T &value, const char *tag);

  /** Read primitive value as one byte from file */
  template<typename T>
  static void readValueAsOneByte(FILE *file, T &value, const char *tag);

  /** Save array of primitve values into file */
  template<typename T>
  static void writeArray(FILE *file, const T *array, size_t item_count, const char *tag);

  /** Read array of primitve values from file */
  template<typename T>
  static void readArray(FILE *file, T *array, size_t item_count, const char *tag);

  /** Save bool as one byte value into file */
  static void writeBool(FILE *file, const bool &value, const char *tag);

  /** Read bool as one byte value from file */
  static void readBool(FILE *file, bool &value, const char *tag);
};

#endif
