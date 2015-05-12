//------------------------------------------------------
#ifndef _PARSEREPLY_H_UID0000034682CF7C13
#define _PARSEREPLY_H_UID0000034682CF7C13

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//------------------------------------------------------

namespace Json
{
	class Value;
};

//------------------------------------------------------

namespace ProbeAPI
{
	struct CountryInfo
	{
		std::string		sCode;
		std::string		sName;
		int				nProbes = 0;

		CountryInfo()
		{}
		CountryInfo(const Json::Value& v);
	};

	struct AsnInfo
	{
		std::string		sId;
		std::string		sName;

		AsnInfo()
		{}
		AsnInfo(const Json::Value& v);
	};

	struct NetworkInfo
	{
		std::string		sName;

		NetworkInfo()
		{}
		NetworkInfo(const Json::Value& v);
	};

	struct PingResultInfo
	{
		bool			bTimeout = false;
		int				nTimeMs = 0;
		int64_t			nId = 0;
		std::string		sUniqueId;

		CountryInfo		country;
		AsnInfo			asn;
		NetworkInfo		network;

		PingResultInfo()
		{}
		PingResultInfo(const Json::Value& v);
	};

	std::vector<ProbeAPI::CountryInfo> ParseCountries(const std::string& sJson);
	std::vector<ProbeAPI::PingResultInfo> ParsePingResults(const std::string& sJson);
}

//------------------------------------------------------
#endif //ifndef _PARSEREPLY_H_UID0000034682CF7C13
