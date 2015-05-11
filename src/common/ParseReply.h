//------------------------------------------------------
#ifndef _PARSEREPLY_H_UID0000034682CF7C13
#define _PARSEREPLY_H_UID0000034682CF7C13

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>

//------------------------------------------------------

namespace ProbeAPI
{
	struct CountryInfo
	{
		std::string		sCountryCode;
		std::string		sCountryName;
		int				nProbes = 0;
	};

	std::vector<ProbeAPI::CountryInfo> ParseCountries(const std::string& sJson);

	struct PingResultInfo
	{
		bool			bTimeout = false;
		int				nTimeMs = 0;
	};

	std::vector<ProbeAPI::PingResultInfo> ParsePingResults(const std::string& sJson);
}

//------------------------------------------------------
#endif //ifndef _PARSEREPLY_H_UID0000034682CF7C13
