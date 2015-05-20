//------------------------------------------------------
#ifndef _OPTIONS_H_UID000003A649B0548E
#define _OPTIONS_H_UID000003A649B0548E

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//------------------------------------------------------

#define DEFAULT_COUNTRY_META			"<biggest>"
#define DEFAULT_PING_COUNTRY_CODE		"CZ"

//------------------------------------------------------

class ApplicationOptions
{
public:
	ApplicationOptions();

	int ProcessCommandLine(const int argc, const char* const argv[]);	// returns non-zero result if process exit required

public:
	bool		bVerbose;
	bool		bDebug;
	uint32_t	nMaxTimeoutMs;
	uint32_t	nCount;
	uint16_t	nPacketSize;
	uint32_t	nTTL;

	enum eMode
	{
		MODE_UNKNOWN,
		MODE_GET_COUNTRIES,
		MODE_GET_ASNS,
		MODE_DO_BY_COUNTRY,
		MODE_DO_BY_ASN,
	};

	eMode		mode;
	std::string	sModeArgument;
	std::string	sTarget;
};

//------------------------------------------------------
#endif //ifndef _OPTIONS_H_UID000003A649B0548E
