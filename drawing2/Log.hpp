/*
* File:   Log.hpp
* Author: darek
*
* Created on 20 December 2017, 13:48
*/
#ifndef QFRACTAL_LOG_HPP
#define QFRACTAL_LOG_HPP

#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <stdexcept>
#include <typeinfo>

#define Q_THROW(_this, _fmt, ...) { \
  char msg[512]; \
  snprintf(msg, sizeof(msg), "[ERROR] %s#%d.%s %s: " _fmt, \
    __FILE__, __LINE__, __FUNCTION__, typeid(_this).name(), ##__VA_ARGS__); \
  throw std::domain_error(msg); \
}

#define Q_STTHROW(_fmt, ...) { \
  char msg[512]; \
  snprintf(msg, sizeof(msg), "[ERROR] %s#%d.%s: " _fmt, \
    __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__); \
  throw std::domain_error(msg); \
}

#define Q_LOG(_level, _fmt, ...) { \
  fprintf(stderr, "[%5s] %s#%d.%s: " _fmt "\n", \
    _level, __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__); \
}

//TODO: Log level filtering support
#define log_error(_fmt, ...) Q_LOG("ERROR", _fmt, ##__VA_ARGS__)
#define log_warn(_fmt, ...) Q_LOG("WARN", _fmt, ##__VA_ARGS__)
#define log_info(_fmt, ...) Q_LOG("INFO", _fmt, ##__VA_ARGS__)
#define log_debug(_fmt, ...) Q_LOG("DEBUG", _fmt, ##__VA_ARGS__)

#endif
