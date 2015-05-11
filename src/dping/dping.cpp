//------------------------------------------------------
#include "stdafx.h"
#include "dping.h"

#include "common/Common.h"
#include "common/ParseReply.h"

#include <json/json.h>

#include <iostream>
#include <iomanip>

#include <algorithm>
#include <thread>
#include <chrono>

using namespace std;

//------------------------------------------------------

string GetDefaultCountryToPing(ProbeApiRequester& requester, const ProgramOptions& options)
{
	ProbeApiRequester::Request request("GetCountries");

	const ProbeApiRequester::Reply reply = requester.DoRequest(request, options.bVerbose);
	if (!reply.bSucceeded)
	{
		throw exception(("GetDefaultCountryToPing: " + reply.sErrorDescription).c_str());
		return string();
	}

	using namespace ProbeAPI;
	std::vector<CountryInfo> countries;

	try
	{
		countries = ParseCountries(reply.sBody);
	}
	catch (exception& e)
	{
		throw exception((string("GetDefaultCountryToPing: ") + e.what()).c_str());
		return string();
	}

	std::sort(countries.begin(), countries.end(), [](const CountryInfo& a, const CountryInfo& b)
	{
		// nProbes DESC
		if (a.nProbes != b.nProbes)
		{
			return a.nProbes > b.nProbes;
		}
		return false;
	});

	if (countries.empty())
	{
		return DEFAULT_PING_COUNTRY_CODE;
	}
	if (countries[0].nProbes < 1)
	{
		return DEFAULT_PING_COUNTRY_CODE;
	}

	return countries[0].sCountryCode;
}

//------------------------------------------------------

int PingByCountry(const ProgramOptions& options)
{
	ProbeApiRequester requester;

	const string& sTarget = options.sTarget;

	cout << endl << "Pinging " << sTarget << ":" << endl << flush;

	string sCountryCode = options.sModeArgument;
	if (DEFAULT_PING_COUNTRY_META == sCountryCode)
	{
		sCountryCode = GetDefaultCountryToPing(requester, options);
	}

	const string sUrl = OSSFMT("StartPingTestByCountry?countrycode=" << sCountryCode
		<< "&destination=" << sTarget
		<< "&probeslimit=" << options.nPingCount
		<< "&timeout=" << options.nMaxTimeoutMs);

	ProbeApiRequester::Request request(sUrl);
	request.nHttpTimeoutSec += options.nMaxTimeoutMs / 1000;

	const ProbeApiRequester::Reply reply = requester.DoRequest(request, options.bVerbose);
	if (!reply.bSucceeded)
	{
		cerr << "ERROR! " << reply.sErrorDescription << endl;
		return eRetCode::ApiFailure;
	}

	using namespace ProbeAPI;
	std::vector<PingResultInfo> items;

	try
	{
		items = ParsePingResults(reply.sBody);
	}
	catch (exception& e)
	{
		cerr << "ERROR! " << e.what() << endl;
		return eRetCode::ApiFailure;
	}

	int64_t nSent = 0;
	int64_t nReceived = 0;
	int nPingMin = INT_MAX;
	int nPingMax = 0;
	int64_t nPingSum = 0;

	for (const auto& info : items)
	{
		if (true)
		{
			cout << flush;
			const int nMaxDelay = 500;
			const int nDelayMs = info.bTimeout ? 500 : info.nTimeMs;
			std::this_thread::sleep_for(std::chrono::milliseconds((min)(nDelayMs, nMaxDelay)));
		}

		++nSent;
		if (info.bTimeout)
		{
			cout << "Request timed out." << endl;
		}
		else
		{
			++nReceived;
			nPingMin = (min)(nPingMin, info.nTimeMs);
			nPingMax = (max)(nPingMax, info.nTimeMs);
			nPingSum += info.nTimeMs;
			cout << "Reply from " << sTarget << ": time=" << info.nTimeMs << "ms" << endl;
		}
	}

	cout << endl << "Ping statistics for " << sTarget << endl;
	cout << "    Packets : Sent = " << nSent << ", Received = " << nReceived << ", Lost = " << (nSent - nReceived) << " (" << ((nSent - nReceived) * 100 / (nSent ? nSent : 1)) << " % loss)," << endl;
	if (nReceived > 0)
	{
		cout << "Approximate round trip times in milli-seconds:" << endl;
		cout << "    Minimum = " << nPingMin << "ms, Maximum = " << nPingMax << "ms, Average = " << (nPingSum / nReceived) << "ms" << endl;
	}

	return eRetCode::OK;
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

	using namespace ProbeAPI;
	std::vector<CountryInfo> items;
		
	try
	{
		items = ParseCountries(reply.sBody);
	}
	catch (exception& e)
	{
		cerr << "ERROR! " << e.what() << endl;
		return eRetCode::ApiFailure;
	}

	std::sort(items.begin(), items.end(), [](const CountryInfo& a, const CountryInfo& b)
	{
		// nProbes DESC
		if (a.nProbes != b.nProbes)
		{
			return a.nProbes > b.nProbes;
		}
		// sCountryName ASC
		if (a.sCountryName != b.sCountryName)
		{
			return a.sCountryName < b.sCountryName;
		}
		return false;
	});

	cout << setw(2) << "ID" << " " << setw(40) << left << "Country Name" << " " << setw(5) << "Number of hosts" << endl;
	cout << setfill('-') << setw(2 + 1 + 40 + 1 + 5 + 10) << "-" << setfill(' ') << endl;

	for (const auto& info : items)
	{
		if (0 == info.nProbes)
		{
			//continue;
		}

		cout << setw(2) << info.sCountryCode << " " << setw(40) << left << info.sCountryName << right << " " << setw(5) << info.nProbes << endl;
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
