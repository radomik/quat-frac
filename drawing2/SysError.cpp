#include "stdafx.h"
#include "SysError.hpp"

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
    char msg_buf[490];
    int _errno = errno;
    char *errno_msg = strerror_r(errno, msg_buf, sizeof(msg_buf));
    if (errno_msg) {
      snprintf(this->buf, sizeof(this->buf), "%d: %s", _errno, errno_msg);
    } else {
      snprintf(this->buf, sizeof(this->buf), "%d: Unknown error", _errno);
    }
    this->msg = &this->buf[0];
  }
#else
  SysError::SysError() {
    snprintf(buf, sizeof(buf), "Unknown error (unsupported platform)");
    msg = buf;
  }
#endif

#ifdef _SYSERROR_TEST
int main(int argc, char** argv) {
  if (argc < 2) {
    printf("Usage: %s <path to file to be opened for reading>\n", argv[0]);
    return 1;
  }
  FILE *f = fopen(argv[1], "rb");
  if (!f) {
    SysError ex;
    printf("Fail to open '%s' [%s]\n", argv[1], ex.msg);
  } else {
    printf("Opened file '%s' for reading\n", argv[1]);

    unsigned int pos = 0;
    while (! feof(f) && pos < 10) {
      unsigned char c;
      pos++;
      int retv = fread(&c, sizeof(c), 1, f);
      if (retv == 0) {
        printf("Found EOF after %u bytes\n", pos);
        break;
      }
      if (retv == 1) {
        printf("%02X (%c) ", c, isascii(c) ? c : '?');

      } else {
        SysError ex;
        printf("\nError reading byte at offset %u [%s]\n", pos-1, ex.msg);
      }
    }
    printf("\n");
    fclose(f);
  }
  return 0;
}
#endif
