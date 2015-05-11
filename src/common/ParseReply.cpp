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
		const Json::Value country = items[index];

		res.push_back(CountryInfo());
		CountryInfo& info = res.back();

		info.sCountryCode = country.get("CountryCode", "").asString();
		info.sCountryName = country.get("CountryName", "").asString();
		info.nProbes = country.get("ProbesCount", 0).asInt();
	}

	return res;
}

//------------------------------------------------------

std::vector<ProbeAPI::PingResultInfo> ProbeAPI::ParsePingResults(const std::string& sJson)
{
	std::vector<PingResultInfo> res;

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

	const Json::Value items = root["StartPingTestByCountryResult"];

	res.reserve(items.size());

	for (size_t index = 0; index < items.size(); ++index)
	{
		const Json::Value country = items[index];

		res.push_back(PingResultInfo());
		PingResultInfo& info = res.back();

		const int nDefaultVal = 9999;
		const Json::Value val = country.get("PingTime", nDefaultVal);

		info.bTimeout = val.isNull();
		if (!val.isNull())
		{
			info.nTimeMs = val.asInt();
		}
		else
		{
			info.nTimeMs = nDefaultVal;
		}
	}

	return res;
}

//------------------------------------------------------
