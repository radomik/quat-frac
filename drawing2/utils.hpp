/*
 * File:   utils.hpp
 * Author: darek
 *
 * Created on 20 November 2014, 01:17
 */
#ifndef QFRACTAL_UTILS_HPP
#define QFRACTAL_UTILS_HPP

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <cmath>
#include <cfloat>
#include <typeinfo>
#include <cassert>
#include <climits>
#include <cstring>
#include <string>
#include "stdafx.h"
#include "SysError.hpp"
#include "Log.hpp"
#include "json.hpp"

extern const char *CO_TRUE_STR, *CO_FALSE_STR, *CO_YES_STR, *CO_NO_STR;

#define sbool(VAL) ( (VAL) ?( CO_TRUE_STR ):( CO_FALSE_STR ) )
#define syesno(VAL) ( (VAL) ?( CO_YES_STR ):( CO_NO_STR ) )

#ifdef _WINDOWS
  #define FILEPATH_SEPARATOR '\\'
  #define FILEPATH_SEPARATOR_STR "\\"
  #define W_FILEPATH_SEPARATOR L'\\'
  #define W_FILEPATH_SEPARATOR_STR L"\\"
#else
  #define FILEPATH_SEPARATOR '/'
  #define FILEPATH_SEPARATOR_STR "/"
  #define W_FILEPATH_SEPARATOR L'/'
  #define W_FILEPATH_SEPARATOR_STR L"/"
#endif

//inline int sys_strcasecmp(const char *a, const char *b);
//inline int sys_wstrcasecmp(const wchar_t *a, const wchar_t *b);
#if defined(WIN32) || defined(_WINDOWS)
  #define sys_wstrcasecmp(a,b) _wcsicmp(a,b)
  #define sys_strcasecmp(a, b) _stricmp(a, b)
#elif defined(_unix_) || defined(_LINUX)
  #define sys_wstrcasecmp(a,b) wcsicmp(a,b)
  #define sys_strcasecmp(a, b) strcasecmp(a, b)
#endif

//inline int sys_snprintf(char *str, size_t size, const char *format, ...);
#if defined(WIN32) || defined(_WINDOWS)
  #define sys_snprintf(str, size, format, ...) _snprintf_s(str, size, _TRUNCATE, format, ##__VA_ARGS__)
#elif defined(_unix_) || defined(_LINUX)
  #define sys_snprintf(str, size, format, ...) snprintf(str, size, format, ##__VA_ARGS__)
#endif

//inline int sys_snwprintf(wchar_t *str, size_t size, const wchar_t *format, ...);
#if defined(WIN32) || defined(_WINDOWS)
#define sys_snwprintf(str, size, format, ...) _snwprintf_s(str, size, _TRUNCATE, format, ##__VA_ARGS__)
#elif defined(_unix_) || defined(_LINUX)
#define sys_snwprintf(str, size, format, ...) snwprintf(str, size, format, ##__VA_ARGS__)
#endif

//inline int sys_sscanf(const char *str, const char *fmt, ...);
#if defined(WIN32) || defined(_WINDOWS)
  #define sys_sscanf(str, fmt, ...) sscanf_s(str, fmt, ##__VA_ARGS__)
#elif defined(_unix_) || defined(_LINUX)
  #define sys_sscanf(str, fmt, ...) sscanf(str, fmt, ##__VA_ARGS__)
#endif

namespace argline {
  bool hasOption(
    int       argc,
    wchar_t*    argv[],
    const wchar_t*  op);
  bool hasOption(
    int       argc,
    char*     argv[],
    const char*   op);

  const wchar_t* getOption(
    int       argc,
    wchar_t*    argv[],
    const wchar_t*  op,
    const wchar_t*  defaultValue = NULL);
  const char* getOption(
    int       argc,
    char*     argv[],
    const char*   op,
    const char*   defaultValue = NULL);

  bool hasHelpOption(int argc, wchar_t* argv[]);
  bool hasHelpOption(int argc, char **argv);
};

/** Replace extension of string
 * @param  s       Filepath/filename to be changed
 * @param  newExt  New extension (with dot) e.g.: ".bin"
 *
 * @note If input string does not contain extension then new extension
 *       is appended to it
 */
void replaceOrAppendExt(std::wstring& s, const std::wstring& newExt);
void replaceOrAppendExt(std::string& s, const std::string& newExt);

/// (public,static)
/// Retrieve filename from filepath
/// @param filepath     Path to file or filename
/// @param filepath_len   Length of filepath (count of characters)
/// @return Index of first character of filename inside filepath
size_t getFileIndex(const char *filepath, size_t filepath_len);
size_t getFileIndex(const wchar_t *filepath, size_t filepath_len);

/// (public,static)
/// Retrieve filename from filepath
/// @param filepath     Path to file or filename
/// @param filepath_len   Length of filepath (count of characters)
/// @return Index of first character of filename inside filepath
inline size_t getFileIndex(const char *filepath) {
  return getFileIndex(filepath, strlen(filepath));
}

inline size_t getFileIndex(const wchar_t *filepath) {
  return getFileIndex(filepath, wcslen(filepath));
}

inline const char *getFilename(const char *filepath, size_t filepath_len) {
  return &filepath[getFileIndex(filepath, filepath_len)];
}

inline const wchar_t *getFilename(const wchar_t *filepath, size_t filepath_len) {
  return &filepath[getFileIndex(filepath, filepath_len)];
}

inline const char *getFilename(const char *filepath) {
  return getFilename(filepath, strlen(filepath));
}

inline const wchar_t *getFilename(const wchar_t *filepath) {
  return getFilename(filepath, wcslen(filepath));
}

/// (public,static)
/// Convert unicode string to multibyte string
/// Returned string must be deleted[] by caller
/// return NULL on error, empty string on conversion error
char* unicodeToMultibyte(const wchar_t *src, size_t src_len);

/// (public,static)
/// Convert unicode string to multibyte string
/// Returned string must be deleted[] by caller
/// return NULL on error, empty string on conversion error
inline char* unicodeToMultibyte(const wchar_t *src) {
  return unicodeToMultibyte(src, wcslen(src));
}

/// Format byte_size
/// byte_size <= 1024 B --> out_str = "<byte_size> B"
/// byte_size <= 1 MB   --> out_str = "<%5.2f byte_size as KB> KB"
/// byte_size  > 1 MB   --> out_str = "<%5.2f byte_size as MB> MB"
const char* formatBytes(size_t byte_size, char *out_str, size_t out_str_size);

#define typnam(var) typeid(var).name()

#endif

