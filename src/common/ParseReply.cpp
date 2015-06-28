//------------------------------------------------------
#include "stdafx.h"
#include "ParseReply.h"

#include "Common.h"

#include <json/json.h>

using namespace std;

//------------------------------------------------------

#ifdef _MSC_VER
//#pragma comment (lib, "jsoncpp.lib")
#endif // _MSC_VER > 1000

//------------------------------------------------------

ProbeAPI::CountryInfo::CountryInfo(const Json::Value& v)
{
	//     {
	//       "CountryCode": "AF",
	//       "CountryFlag": "http://speedcheckerapi.blob.core.windows.net/bsc-img-country-logos/af.png",
	//       "CountryName": "Afghanistan",
	//       "ProbesCount": 5
	//     },

	sCode = v.get("CountryCode", "").asString();
	sName = v.get("CountryName", "").asString();
	nProbes = v.get("ProbesCount", 0).asInt();
}

//------------------------------------------------------

ProbeAPI::AsnInfo::AsnInfo(const Json::Value& v)
{
	//     {
	//     "AsnID": "AS6830",
	//     "AsnName": "Liberty Global Operations B.V."
	//     },

	sId = v.get("AsnID", "").asString();
	sName = v.get("AsnName", "").asString();
}

//------------------------------------------------------

ProbeAPI::NetworkInfo::NetworkInfo(const Json::Value& v)
{
	//     {
	//     "LogoURL": null,
	//     "NetworkID": 31,
	//     "NetworkName": "UPC Ceska Republica, s.r.o."
	//     },

	sName = v.get("NetworkName", "").asString();
}

//------------------------------------------------------

ProbeAPI::PingResult::PingResult(const Json::Value& v)
{
	// Ping results:
	// "PingTime": 35,
	// "PingTime": null, (OLD)
	// "PingTime": "TimedOut", (NEW)
	// Tracert results:
	// "Ping2":"1",
	// "Ping3":"-",
	const int nDefaultVal = 9999;

	bTimeout = false;
	nTimeMs = nDefaultVal;

	if (v.isNull())
	{
		bTimeout = true;
		nTimeMs = nDefaultVal;
	}
	else if(v.isNumeric())
	{
		nTimeMs = v.asInt();
	}
	else if (v.isString())
	{
		const string sVal = v.asString();
		if (sVal == "-" || sVal == "TimedOut")
		{
			bTimeout = true;
			nTimeMs = nDefaultVal;
		}
		else if (!sVal.empty() && sVal[0] >= '0' && sVal[0] <= '9')
		{
			nTimeMs = stoi(sVal);
		}
		else
		{
			throw PException(eRetCode::ApiParsingFail) << "Failed parsing ping result: type = " << v.type() << "; value = " << v;
		}
	}
	else
	{
		throw PException(eRetCode::ApiParsingFail) << "Failed parsing ping result: type = " << v.type() << "; value = " << v;
	}
}

//------------------------------------------------------

ProbeAPI::TracertHopResults::TracertHopResults(const Json::Value& v)
{
	// {"Ping1":"26","Ping2":"34","Ping3":"29","Url":"78.254.249.170"},
	sReplyHost = v.get("Url", "").asString();

	for (int i = 0;; ++i)
	{
		const string sName = OSSFMT("Ping" << (i+1));
		if (!v.isMember(sName))
		{
			break;
		}
		if (vectResults.empty())
		{
			vectResults.reserve(3);
		}
		vectResults.emplace_back(v.get(sName, Json::Value::null));
	}
}

//------------------------------------------------------

ProbeAPI::TracerouteInfo::TracerouteInfo(const Json::Value& v)
{
	// [
	// 	{
	// 		"Destination": "www.onet.pl",
	// 		"Tracert" :
	// 		[
	// 			{
	// 				"Ping1": "12",
	// 					"Ping2" : "2",
	// 					"Ping3" : "3",
	// 					"Url" : "192.168.0.254"
	// 			},
	// 			{
	// 				"Ping1": "123",
	// 				"Ping2" : "59",
	// 				"Ping3" : "50",
	// 				"Url" : "82.245.112.254"
	// 			},
	// 			.............
	// 			{
	// 				"Ping1": "100",
	// 					"Ping2" : "99",
	// 					"Ping3" : "103",
	// 					"Url" : "213.180.141.140"
	// 			}
	// 		]
	// 	}
	// ]

	if (!v.isArray() || v.size() < 1)
		return;

	const Json::Value v2 = v[0];

	sTarget = v2.get("Destination", "").asString();
	const Json::Value v3 = v2.get("Tracert", "");

	const Json::ArrayIndex n = v3.size();
	assert(v3.isArray());
	vectHops.reserve(n);

	for (Json::ArrayIndex i = 0; i < n; ++i)
	{
		vectHops.emplace_back(v3[i]);	// create TracertHopResults()
	}
}

//------------------------------------------------------

ProbeAPI::ProbeInfo::ProbeInfo(const Json::Value& v, const eParseMode mode)
{
	//     {
	//       "ASN": {
	//         "AsnID": "AS6830",
	//         "AsnName": "Liberty Global Operations B.V."
	//       },
	//       "Country": {
	//         "CountryCode": "CZ",
	//         "CountryFlag": "http://speedcheckerapi.blob.core.windows.net/bsc-img-country-logos/cz.png",
	//         "CountryName": "Czech Republic"
	//       },
	//       "DateTimeStamp": "/Date(1429519784633+0000)/",
	//       "ID": 57904,
	//       "Location": {
	//         "Latitude": 50.130981,
	//         "Longitude": 14.466462
	//       },
	//       "Network": {
	//         "LogoURL": null,
	//         "NetworkID": 31,
	//         "NetworkName": "UPC Ceska Republica, s.r.o."
	//       },
	//       "PingTime": 35
	//     },

	if (ProbeList_AsnOnly == mode)
	{
		asn = AsnInfo(v.get("ASN", ""));
		return;
	}

	ping = PingResult(v.get("PingTime", Json::Value::null));

	nId = v.get("ID", 0).asInt64();

	country = CountryInfo(v.get("Country", ""));
	asn = AsnInfo(v.get("ASN", ""));
	network = NetworkInfo(v.get("Network", ""));

	if (ProbeList_TracertResults == mode)
	{
		tracert = TracerouteInfo(v.get("TRACERoute", ""));
	}
}

//------------------------------------------------------

std::string ProbeAPI::ProbeInfo::GetProbeInfo(const bool bAsnIsKnown) const
{
#if 1
	if (bAsnIsKnown)
	{
		return OSSFMT("probe ID " << nId << " net \"" << network.sName << "\" (" << country.sCode << ")");
	}
	else
	{
		return OSSFMT("probe ID " << nId << " net \"" << network.sName << "\" (" << asn.sId << ")");
	}
#else
	//return sUniqueId + " (" + network.sName + ")";
	if (bAsnIsKnown)
	{
		return "net \"" + network.sName + "\" (" + country.sName + ")";
	}
	else
	{
		return asn.sId + " \"" + asn.sName + "\" (" + country.sCode + ")";
	}
#endif
}

//------------------------------------------------------

std::vector<ProbeAPI::CountryInfo> ProbeAPI::ParseCountries(const std::string& sJson)
{
	Json::Reader reader;
	Json::Value root;
	const bool parsedOK = reader.parse(sJson, root);
	if (!parsedOK)
	{
		throw PException(eRetCode::ApiParsingFail) << "Failed parsing json: " << reader.getFormattedErrorMessages();
	}

	// {
	//   "GetCountriesResult": [
	//     {
	//       "CountryCode": "AF",
	//       "CountryFlag": "http://speedcheckerapi.blob.core.windows.net/bsc-img-country-logos/af.png",
	//       "CountryName": "Afghanistan",
	//       "ProbesCount": 5
	//     },
	//     {
	//       "CountryCode": "AL",
	//       "CountryFlag": "http://speedcheckerapi.blob.core.windows.net/bsc-img-country-logos/al.png",
	//       "CountryName": "Albania",
	//       "ProbesCount": 14
	//     },

	const Json::Value items = root["GetCountriesResult"];

	assert(items.isArray());
	const Json::ArrayIndex n = items.size();
	std::vector<CountryInfo> res;
	res.reserve(n);

	for (Json::ArrayIndex index = 0; index < n; ++index)
	{
		const Json::Value item = items[index];
		res.emplace_back(item);		// create CountryInfo()
	}

	return res;
}

//------------------------------------------------------

namespace ProbeAPI
{
	std::vector<ProbeAPI::ProbeInfo> ParseProbeList(const std::string& sJson, const std::string& sJsonRootItemName, const eParseMode mode);
}

//------------------------------------------------------

std::vector<ProbeAPI::ProbeInfo> ProbeAPI::ParseProbeList(const std::string& sJson, const std::string& sJsonRootItemName, const eParseMode mode)
{
	std::vector<ProbeInfo> res;

	Json::Reader reader;
	Json::Value root;
	const bool parsedOK = reader.parse(sJson, root);
	if (!parsedOK)
	{
		throw PException(eRetCode::ApiParsingFail) << "Failed parsing json: " << reader.getFormattedErrorMessages();
	}

	// {
	//   "StartPingTestByCountryResult": [
	//     {
	//       "ASN": {
	//         "AsnID": "AS6830",
	//         "AsnName": "Liberty Global Operations B.V."
	//       },
	//       "Country": {
	//         "CountryCode": "CZ",
	//         "CountryFlag": "http://speedcheckerapi.blob.core.windows.net/bsc-img-country-logos/cz.png",
	//         "CountryName": "Czech Republic"
	//       },
	//       "DateTimeStamp": "/Date(1429519784633+0000)/",
	//       "ID": 57904,
	//       "Location": {
	//         "Latitude": 50.130981,
	//         "Longitude": 14.466462
	//       },
	//       "Network": {
	//         "LogoURL": null,
	//         "NetworkID": 31,
	//         "NetworkName": "UPC Ceska Republica, s.r.o."
	//       },
	//       "PingTime": 35
	//     },
	//     {
	//       "ASN": {
	//         "AsnID": "AS6830",
	//         "AsnName": "Liberty Global Operations B.V."
	//       },

	assert(root.isMember(sJsonRootItemName));
	const Json::Value items = root[sJsonRootItemName];

	assert(items.isArray());
	const Json::ArrayIndex n = items.size();
	res.reserve(n);

	for (Json::ArrayIndex index = 0; index < n; ++index)
	{
		const Json::Value item = items[index];
		res.emplace_back(item, mode);	// create ProbeInfo
		if (g_bTerminateProgram)
		{
			throw PException("ParseProbeList: Terminate program");
		}
	}

	return res;
}

//------------------------------------------------------

std::vector<ProbeAPI::ProbeInfo> ProbeAPI::ParseGetProbesByCountryResult_AsnOnly(const std::string& sJson)
{
	return ParseProbeList(sJson, "GetProbesByCountryResult", ProbeList_AsnOnly);
}

//------------------------------------------------------

std::vector<ProbeAPI::ProbeInfo> ProbeAPI::ParsePingResults(const std::string& sJson, const std::string& sJsonRootItemName)
{
	return ParseProbeList(sJson, sJsonRootItemName, ProbeList_PingResults);
}

//------------------------------------------------------

std::vector<ProbeAPI::ProbeInfo> ProbeAPI::ParseTracertResults(const std::string& sJson, const std::string& sJsonRootItemName)
{
	return ParseProbeList(sJson, sJsonRootItemName, ProbeList_TracertResults);
}

//------------------------------------------------------

