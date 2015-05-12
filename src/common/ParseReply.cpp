//------------------------------------------------------
#include "stdafx.h"
#include "ParseReply.h"

#include "Common.h"

#include <json/json.h>

using namespace std;

//------------------------------------------------------

#ifdef _MSC_VER
#pragma comment (lib, "jsoncpp.lib")
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

ProbeAPI::ProbeInfo::ProbeInfo(const Json::Value& v)
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
	//       "JID": "7427981831429519172968639@performancetests.pcspeedup.com/cz|1.0.14.0|acadeec2-5c2f-4c28-96d0-a6fd72985f8d|0||50.130981|14.466464|4|58",
	//       "Location": {
	//         "Latitude": 50.130981,
	//         "Longitude": 14.466462
	//       },
	//       "Network": {
	//         "LogoURL": null,
	//         "NetworkID": 31,
	//         "NetworkName": "UPC Ceska Republica, s.r.o."
	//       },
	//       "PingTime": 35,
	//       "UniqueID": "acadeec2-5c2f-4c28-96d0-a6fd72985f8d"
	//     },

	const int nDefaultVal = 9999;
	const Json::Value valPingTime = v.get("PingTime", nDefaultVal);

	bTimeout = valPingTime.isNull();
	if (!bTimeout)
	{
		nTimeMs = valPingTime.asInt();
	}
	else
	{
		nTimeMs = nDefaultVal;
	}

	nId = v.get("ID", 0).asInt64();
	sUniqueId = v.get("UniqueID", 0).asString();

	country = CountryInfo(v.get("Country", ""));
	asn = AsnInfo(v.get("ASN", ""));
	network = NetworkInfo(v.get("Network", ""));
}

//------------------------------------------------------

std::vector<ProbeAPI::CountryInfo> ProbeAPI::ParseCountries(const std::string& sJson)
{
	std::vector<CountryInfo> res;

	Json::Reader reader;
	Json::Value root;
	const bool parsedOK = reader.parse(sJson, root);
	if (!parsedOK)
	{
		throw exception(OSSFMT("Failed parsing json: " << reader.getFormattedErrorMessages()).c_str());
		return res;
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

	res.reserve(items.size());

	for (size_t index = 0; index < items.size(); ++index)
	{
		const Json::Value item = items[index];
		res.emplace_back(item);
	}

	return res;
}

//------------------------------------------------------

std::vector<ProbeAPI::ProbeInfo> ProbeAPI::ParseProbeList(const std::string& sJson, const std::string& sJsonRootItemName)
{
	std::vector<ProbeInfo> res;

	Json::Reader reader;
	Json::Value root;
	const bool parsedOK = reader.parse(sJson, root);
	if (!parsedOK)
	{
		throw exception(OSSFMT("Failed parsing json: " << reader.getFormattedErrorMessages()).c_str());
		return res;
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
	//       "JID": "7427981831429519172968639@performancetests.pcspeedup.com/cz|1.0.14.0|acadeec2-5c2f-4c28-96d0-a6fd72985f8d|0||50.130981|14.466464|4|58",
	//       "Location": {
	//         "Latitude": 50.130981,
	//         "Longitude": 14.466462
	//       },
	//       "Network": {
	//         "LogoURL": null,
	//         "NetworkID": 31,
	//         "NetworkName": "UPC Ceska Republica, s.r.o."
	//       },
	//       "PingTime": 35,
	//       "UniqueID": "acadeec2-5c2f-4c28-96d0-a6fd72985f8d"
	//     },
	//     {
	//       "ASN": {
	//         "AsnID": "AS6830",
	//         "AsnName": "Liberty Global Operations B.V."
	//       },

	const Json::Value items = root[sJsonRootItemName];

	res.reserve(items.size());

	for (size_t index = 0; index < items.size(); ++index)
	{
		const Json::Value item = items[index];
		res.emplace_back(item);
	}

	return res;
}

//------------------------------------------------------

std::vector<ProbeAPI::ProbeInfo> ProbeAPI::ParsePingTestByCountryResult(const std::string& sJson)
{
	return ParseProbeList(sJson, "StartPingTestByCountryResult");
}

//------------------------------------------------------

std::vector<ProbeAPI::ProbeInfo> ProbeAPI::ParseGetProbesByCountryResult(const std::string& sJson)
{
	return ParseProbeList(sJson, "GetProbesByCountryResult");
}

//------------------------------------------------------
