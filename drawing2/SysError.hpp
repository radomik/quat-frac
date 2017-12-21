/*
* File:   SysError.hpp
* Author: darek
*
* Created on 9 December 2014, 20:00
*/
#ifndef QFRACTAL_SYSERROR_HPP
#define QFRACTAL_SYSERROR_HPP

#include "utils.hpp"

class SysError {
  char    buf[512];

public:
  const char  *msg;
  SysError();
};

#endif /* QFRACTAL_SYSERROR_HPP */
