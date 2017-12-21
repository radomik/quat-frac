#include "stdafx.h"
#include "SysError.hpp"

#ifndef SYSERROR_INLINE
  #include <errno.h>
  #include <cstring>
  #include <cstdio>

  #ifdef _WINDOWS
    SysError::SysError() {
      if (strerror_s(buf, sizeof(buf), errno) != 0) {
        _snprintf_s(buf, sizeof(buf), _TRUNCATE, "Unknown error");
      }
      msg = buf;
    }
  #elif _LINUX
    SysError::SysError() {
      msg = strerror_r(errno, buf, sizeof(buf));
    }
  #else
    SysError::SysError() {
      snprintf(buf, sizeof(buf), "Unknown error (unsupported platform)");
      msg = buf;
    }
  #endif
#endif
