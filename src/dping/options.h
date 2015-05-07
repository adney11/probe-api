//------------------------------------------------------
#ifndef _OPTIONS_H_UID000003A649B0548E
#define _OPTIONS_H_UID000003A649B0548E

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>

//------------------------------------------------------

class ProgramOptions
{
public:
	ProgramOptions();

	int ProcessCommandLine(const int argc, const char* const argv[]);	// returns non-zero result if process exit required

public:
	bool	bVerbose;
};

//------------------------------------------------------
#endif //ifndef _OPTIONS_H_UID000003A649B0548E
