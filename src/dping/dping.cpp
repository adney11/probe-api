//------------------------------------------------------
#include "stdafx.h"
#include "dping.h"

#include "common/Common.h"

#include <json/json.h>

#include <iostream>
#include <iomanip>

using namespace std;

//------------------------------------------------------

void HttpReplyDebugPrint(const ProbeApiRequester::Reply &reply)
{
	cout << "request succeeded: " << reply.bSucceeded << endl;
	cout << "request error desc: " << reply.sErrorDescription << endl;
	cout << "reply HTTP code: " << reply.nHttpCode << endl;
	cout << "reply EffectiveUrl: " << reply.sEffectiveUrl << endl;
	cout << "reply Content-Type: " << reply.sContentType << endl;
	cout << "reply body length: " << reply.sBody.length() << endl;

	cout << "REPLY BODY: [[[" << reply.sBody << "]]]" << endl;
}

//------------------------------------------------------

int PingByCountry(const ProgramOptions& options)
{
	cerr << "ERROR! This program mode is not implemented yet!" << endl;
	return eRetCode::NotSupported;
}

//------------------------------------------------------

int PingByAsn(const ProgramOptions& options)
{
	cerr << "ERROR! This program mode is not implemented yet!" << endl;
	return eRetCode::NotSupported;
}

//------------------------------------------------------

int ListCountries(const ProgramOptions& options)
{
	ProbeApiRequester requester;

	ProbeApiRequester::Request request("GetCountries");

	const ProbeApiRequester::Reply reply = requester.DoRequest(request, options.bVerbose);
	if (!reply.bSucceeded)
	{
		cerr << "ERROR! " << reply.sErrorDescription << endl;
		return eRetCode::ApiFailure;
	}

	Json::Reader reader;
	Json::Value root;
	const bool parsedOK = reader.parse(reply.sBody, root);
	if (!parsedOK)
	{
		cerr << "ERROR! Failed parsing json: " << reader.getFormattedErrorMessages() << endl;
		return eRetCode::ApiFailure;
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

	cout << setw(2) << "ID" << " " << setw(40) << left << "Country Name" << " " << setw(5) << "Number of hosts" << endl;
	cout << setfill('-') << setw(2 + 1 + 40 + 1 + 5 + 10) << "-" << setfill(' ') << endl;

	const Json::Value countries = root["GetCountriesResult"];
	for (size_t index = 0; index < countries.size(); ++index)
	{
		const Json::Value country = countries[index];
		const string sCountryCode = country.get("CountryCode", "").asString();
		const string sCountryName = country.get("CountryName", "").asString();
		const int nProbes = country.get("ProbesCount", 0).asInt();

		if (nProbes > 0)
		{
			cout << setw(2) << sCountryCode << " " << setw(40) << left << sCountryName << right << " " << setw(5) << nProbes << endl;
		}
	}

	return eRetCode::OK;
}

//------------------------------------------------------

int ListAsns(const ProgramOptions& options)
{
	cerr << "ERROR! This program mode is not implemented yet!" << endl;
	return eRetCode::NotSupported;
}

//------------------------------------------------------

int Dping(const ProgramOptions& options)
{
	switch (options.mode)
	{
	case ProgramOptions::MODE_PING_BY_COUNTRY:
		return PingByCountry(options);
	case ProgramOptions::MODE_PING_BY_ASN:
		return PingByAsn(options);
	case ProgramOptions::MODE_GET_COUNTRIES:
		return ListCountries(options);
	case ProgramOptions::MODE_GET_ASNS:
		return ListAsns(options);
	default:
		cerr << "ERROR! Unknown program mode " << options.mode << endl;
		return eRetCode::NotSupported;
	}
}

//------------------------------------------------------
