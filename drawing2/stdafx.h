// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//



#ifdef _WINDOWS
#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#endif

#ifndef QFRACTAL_STDAFX_H
#define QFRACTAL_STDAFX_H
// TODO: reference additional headers your program requires here

/** Uncomment this line to enable execution time counting */
#define _USE_TIMING 1

/** Windows target operating system --> _WINDOWS should be defined */
#if (defined(WIN32)) && (! defined(_WINDOWS))
  #define _WINDOWS 1
#endif

/** Linux target operating system --> _LINUX should be defined */
#if (defined(__unix__)) && (! defined(_LINUX))
  #define _LINUX 1
#endif

/** Unicode (UTF-16) command line arguments --> _UTF16 should be defined */
#if (defined(UNICODE)) && (! defined(_UTF16))
  #define _UTF16 1
#endif

/** Check operating system directives */
#if ((! defined(_WINDOWS)) && (! defined(_LINUX))) || ((defined(_WINDOWS)) && (defined(_LINUX)))
  #error "Unknown target platform"
  #error "Please add either _WINDOWS or _LINUX directives to compiler options:"
  #error "    For GCC: -D _LINUX or -D _WINDOWS"
  #error "    For Microsoft Visual Studio: project configuration (_WINDOWS)"
#elif defined(_WINDOWS)
  //#pragma message( "[INFO] Target platform is: Windows" )
#elif defined(_LINUX)
  //#pragma message( "[INFO] Target platform is: Linux" )
#endif

#if defined(UNICODE)
  #if defined(_WINDOWS)
    //#pragma message( "[INFO] Target platform supports UNICODE (UTF-16 on Windows)" )
  #elif defined(_LINUX)
    //#pragma message( "[INFO] Target platform supports UNICODE (Linux)" )
  #endif
#endif

#endif /* QFRACTAL_STDAFX_H */
