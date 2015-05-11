//------------------------------------------------------
#ifndef _OPTIONS_H_UID000003A649B0548E
#define _OPTIONS_H_UID000003A649B0548E

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>
#include <stdint.h>

//------------------------------------------------------

class ProgramOptions
{
public:
	ProgramOptions();

	int ProcessCommandLine(const int argc, const char* const argv[]);	// returns non-zero result if process exit required

public:
	bool		bVerbose;
	uint32_t	nMaxTimeoutMs;
	uint32_t	nPingCount;

	enum eMode
	{
		MODE_UNKNOWN,
		MODE_GET_COUNTRIES,
		MODE_GET_ASNS,
		MODE_PING_BY_COUNTRY,
		MODE_PING_BY_ASN,
	};

	eMode		mode;
	std::string	sModeArgument;
	std::string	sTarget;
};

#define DEFAULT_PING_COUNTRY_STR	"<default>"

//------------------------------------------------------
#endif //ifndef _OPTIONS_H_UID000003A649B0548E
