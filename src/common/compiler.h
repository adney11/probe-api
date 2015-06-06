//------------------------------------------------------
#ifndef _COMPILER_H_UID0000060859C05A14
#define _COMPILER_H_UID0000060859C05A14

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//------------------------------------------------------

#if defined(RC_INVOKED) && !defined(WIN32)
#define WIN32	// this is used during .rc file compilation in Windows
#endif

#if defined(__linux__)
#define DEST_OS_LINUX
#elif defined(WIN32)
#define DEST_OS_WINDOWS
#else
#error Unsupported OS!
#endif

#ifdef DEST_OS_WINDOWS
// _WIN32_WINNT should not be needed since we don't include Windows.h, but you never know how program will mutate in the future.
#ifndef _WIN32_WINNT            // Specifies that the minimum required platform
#define _WIN32_WINNT 0x0500     // 0x0500 == Windows 2000
#endif
#endif

//------------------------------------------------------
#endif //ifndef _COMPILER_H_UID0000060859C05A14
