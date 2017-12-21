/*
* File:   SysError.hpp
* Author: darek
*
* Created on 9 December 2014, 20:00
*/
#ifndef QFRACTAL_SYSERROR_H
#define QFRACTAL_SYSERROR_H

#include "utils.hpp"

#define SYSERROR_INLINE 1

#ifdef SYSERROR_INLINE
  #include <errno.h>
  #include <cstring>
  #include <cstdio>
#endif

class SysError {
  char    buf[256];

public:
  const char  *msg;

#ifdef SYSERROR_INLINE
  #include <errno.h>
  #include <cstring>
  #include <cstdio>
  #ifdef _WINDOWS
    SysError() {
      if (strerror_s(buf, sizeof(buf), errno) != 0) {
        _snprintf_s(buf, sizeof(buf), _TRUNCATE, "Unknown error");
      }
      msg = buf;
    }
  #elif _LINUX
    SysError() {
      msg = strerror_r(errno, buf, sizeof(buf));
    }
  #else
    SysError() {
      snprintf(buf, sizeof(buf), "Unknown error (unsupported platform)");
      msg = buf;
    }
  #endif
#else
  SysError();
#endif
};

#endif /* QFRACTAL_GENERATOR_SYSERROR_H */
