//------------------------------------------------------
#ifndef _COMMON_OPERATIONS_H_UID00000928MW81
#define _COMMON_OPERATIONS_H_UID00000928MW81

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>

//------------------------------------------------------

#define DEFAULT_COUNTRY_META			"<biggest>"
#define DEFAULT_PING_COUNTRY_CODE		"CZ"

class ProbeApiRequester;
namespace ProbeAPI
{
	struct PingResult;
};

//------------------------------------------------------

struct CommonOptions
{
	const bool&			bDebug;
	const std::string&	sModeArgument;	// Country code in --list-asn mode

	CommonOptions(const bool bDebug_, const std::string& sModeArgument_):
		bDebug(bDebug_), sModeArgument(sModeArgument_)
	{}
};

//------------------------------------------------------

void DoSleep(const ProbeAPI::PingResult& ping, bool& bFirstSleep);
std::string GetDefaultSourceCountry(ProbeApiRequester& requester, const CommonOptions& options);

int ListCountries(const CommonOptions& options);
int ListAsns(const CommonOptions& options);

//------------------------------------------------------
#endif //ifndef _COMMON_OPERATIONS_H_UID00000928MW81
