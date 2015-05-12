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
	enum eParseMode
	{
		ProbeList_All,
		ProbeList_AsnOnly,
	};

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

	struct ProbeInfo
	{
		bool			bTimeout = false;
		int				nTimeMs = 0;
		int64_t			nId = 0;
		std::string		sUniqueId;

		CountryInfo		country;
		AsnInfo			asn;
		NetworkInfo		network;

		ProbeInfo()
		{}
		ProbeInfo(const Json::Value& v, const eParseMode mode);
	};

	std::vector<ProbeAPI::CountryInfo> ParseCountries(const std::string& sJson);

	std::vector<ProbeAPI::ProbeInfo> ParsePingTestByCountryResult(const std::string& sJson);
	std::vector<ProbeAPI::ProbeInfo> ParseGetProbesByCountryResult_AsnOnly(const std::string& sJson);
}

//------------------------------------------------------
#endif //ifndef _PARSEREPLY_H_UID0000034682CF7C13
