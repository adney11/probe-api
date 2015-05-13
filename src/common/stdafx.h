// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//------------------------------------------------------

#include <stdint.h>

#include <iostream>
#include <iomanip>
#include <memory>

#include <sstream>
#define OSSFMT(expr)	(static_cast<std::ostringstream&>(std::ostringstream() << expr).str())

#include <string>
#include <vector>
#include <list>
#include <map>

//------------------------------------------------------

#if defined(__linux__)
#define OS_LINUX
#elif defined(_WIN32)
#define OS_WINDOWS
#else
#error Unsupported OS!
#endif

#ifdef OS_WINDOWS
// _WIN32_WINNT should not be needed since we don't include Windows.h, but you never know how program will mutate in the future.
#ifndef _WIN32_WINNT            // Specifies that the minimum required platform
#define _WIN32_WINNT 0x0500     // 0x0500 == Windows 2000
#endif
#endif

//------------------------------------------------------

// TODO: reference additional headers your program requires here
