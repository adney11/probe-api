//------------------------------------------------------
#include "stdafx.h"
#include "ParseReply.h"

#include "Common.h"

#include <json/json.h>
// making a change here just to check
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

ProbeAPI::PingInfo::PingInfo(const Json::Value& v)
{
	// 	{
	// 		"Status":"OK",
	// 		"Destination":null,			// or "google-public-dns-a.google.com"
	// 		"HostName":null,			// or "google-public-dns-a.google.com"
	// 		"IP":"8.8.8.8",
	// 		"PingTime":35,				// average
	// 		"PingTimeArray":[
	// 			"35",
	// 			"34",
	// 			"36"
	// 		]
	// 	}

	if (!v.isArray() || v.size() < 1)
		return;

	const Json::Value v2 = v[0];

	sTargetHost = v2.get("HostName", "").asString();
	sTargetIp = v2.get("IP", "").asString();

	const Json::Value v3 = v2.get("PingTimeArray", "");

	const Json::ArrayIndex n = v3.size();
	assert(v3.isArray());
	vectResults.reserve(n);

	for (Json::ArrayIndex i = 0; i < n; ++i)
	{
		vectResults.emplace_back(v3[i]);	// creating PingResult()
	}
}

//------------------------------------------------------

ProbeAPI::TracertHopResults::TracertHopResults(const Json::Value& v)
{
	// {"HostName":"google-public-dns-a.google.com","IP":"8.8.8.8","PingTimeArray":"[\"50\",\"49\",\"50\"]","Ping1":"26","Ping2":"34","Ping3":"29","Status":"OK"},

	sReplyHost = v.get("HostName", "").asString();
	sReplyIp = v.get("IP", "").asString();

#if 1
	const Json::Value v2 = v.get("PingTimeArray", "");

	const Json::ArrayIndex n = v2.size();
	assert(v2.isArray());
	vectResults.reserve(n);

	for (Json::ArrayIndex i = 0; i < n; ++i)
	{
		vectResults.emplace_back(v2[i]);	// creating PingResult()
	}
#else
	for (int i = 0;; ++i)
	{
		const string sName = OSSFMT("Ping" << (i + 1));
		if (!v.isMember(sName))
		{
			break;
		}
		if (vectResults.empty())
		{
			vectResults.reserve(3);
		}
		vectResults.emplace_back(v.get(sName, Json::Value::null));	// creating PingResult()
	}
#endif
}

//------------------------------------------------------

ProbeAPI::TracerouteInfo::TracerouteInfo(const Json::Value& v)
{
	// "TRACERoute":[
	// 
	// 	{
	// 		"Destination":"8.8.8.8",
	// 		"HostName":"google-public-dns-a.google.com",
	// 		"IP":"8.8.8.8",
	// 		"Tracert":[
	// 			{
	// 				"HostName":"WER-FG-ABC.lo",
	// 				"IP":"192.168.1.1",
	// 				"PingTimeArray":"[\"0\",\"0\",\"0\"]",
	// 				"Ping1":"0",
	// 				"Ping2":"0",
	// 				"Ping3":"0",
	// 				"Status":"OK"
	// 			},
	// ....
	// 			{
	// 				"HostName":"",
	// 				"IP":"216.239.47.147",
	// 				"PingTimeArray":"[null,null,null]",
	// 				"Ping1":null,
	// 				"Ping2":null,
	// 				"Ping3":null,
	// 				"Status":"Timeout"
	// 			},
	// 			{
	// 				"HostName":"google-public-dns-a.google.com",
	// 				"IP":"8.8.8.8",
	// 				"PingTimeArray":"[\"50\",\"49\",\"50\"]",
	// 				"Ping1":"50",
	// 				"Ping2":"49",
	// 				"Ping3":"50",
	// 				"Status":"OK"
	// 			}
	// 		]
	// 	}
	// ]

	if (!v.isArray() || v.size() < 1)
		return;

	const Json::Value v2 = v[0];

	sTargetHost = v2.get("HostName", "").asString();
	sTargetIp = v2.get("IP", "").asString();

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
	// 	{
	// 		"ASN": {
	// 		"AsnID": "AS6830",
	// 		"AsnName": "Liberty Global Operations B.V."
	// 		},
	// 		"Country": {
	// 		"CountryCode": "CZ",
	// 		"CountryFlag": "http://speedcheckerapi.blob.core.windows.net/bsc-img-country-logos/cz.png",
	// 		"CountryName": "Czech Republic"
	// 		},
	// 		"DateTimeStamp": "/Date(1429519784633+0000)/",
	// 		"ID": 57904,
	// 		"Location": {
	// 		"Latitude": 50.130981,
	// 		"Longitude": 14.466462
	// 		},
	// 		"Network": {
	// 		"LogoURL": null,
	// 		"NetworkID": 31,
	// 		"NetworkName": "UPC Ceska Republica, s.r.o."
	// 		},
	// 		"Ping":[
	// 			{
	// 				"Status":"OK",
	// 				"Destination":null,			// or "google-public-dns-a.google.com"
	// 				"HostName":null,			// or "google-public-dns-a.google.com"
	// 				"IP":"8.8.8.8",
	// 				"PingTime":35,				// average
	// 				"PingTimeArray":[
	// 					"35",
	// 					"34",
	// 					"36"
	// 				]
	// 			}
	// 		],
	// 		"PingTime": 35				// average
	//		"TRACERoute":[
	// ....
	//		]
	// 	},

	if (ProbeList_AsnOnly == mode)
	{
		asn = AsnInfo(v.get("ASN", ""));
		return;
	}

	nId = v.get("ID", 0).asInt64();

	country = CountryInfo(v.get("Country", ""));
	asn = AsnInfo(v.get("ASN", ""));
	network = NetworkInfo(v.get("Network", ""));

	if (ProbeList_PingResults == mode)
	{
		ping = PingInfo(v.get("Ping", ""));
	}

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

