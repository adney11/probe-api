//------------------------------------------------------
#ifndef _LIST_ITEMS_H_UID00000928173JQMW81
#define _LIST_ITEMS_H_UID00000928173JQMW81

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>

//------------------------------------------------------

#define DEFAULT_COUNTRY_META			"<biggest>"
#define DEFAULT_PING_COUNTRY_CODE		"CZ"

//------------------------------------------------------

class ProbeApiRequester;

struct CommonOptions
{
	bool		bDebug = false;
	std::string	sModeArgument;	// Country code in --list-asn mode

	CommonOptions()
	{}
	CommonOptions(const bool bDebug_, const std::string& sModeArgument_):
		bDebug(bDebug_), sModeArgument(sModeArgument_)
	{}
};

//------------------------------------------------------

std::string GetDefaultSourceCountry(ProbeApiRequester& requester, const CommonOptions& options);

int ListCountries(const CommonOptions& options);
int ListAsns(const CommonOptions& options);

//------------------------------------------------------
#endif //ifndef _LIST_ITEMS_H_UID00000928173JQMW81
