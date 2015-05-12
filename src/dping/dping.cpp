//------------------------------------------------------
#include "stdafx.h"
#include "dping.h"

#include "common/config.h"					// for PROBEAPI_PING_PACKET_SIZE, PROBEAPI_PING_TTL
#include "common/Common.h"
#include "common/ParseReply.h"

#include <json/json.h>

#include <algorithm>
#include <thread>
#include <chrono>

using namespace std;

//------------------------------------------------------

string GetDefaultCountryToPing(ProbeApiRequester& requester, const ProgramOptions& options)
{
	ProbeApiRequester::Request request("GetCountries");

	const ProbeApiRequester::Reply reply = requester.DoRequest(request, options.bDebug);
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

	return countries[0].sCode;
}

//------------------------------------------------------

struct PingingStats
{
	int64_t	nSent = 0;
	int64_t	nReceived = 0;
	int		nPingMin = INT_MAX;
	int		nPingMax = 0;
	int64_t	nPingSum = 0;
};

//------------------------------------------------------


int MakePackOfPingsByCountry(const string& sCountryCode, const string& sTarget, const ProgramOptions& options, ProbeApiRequester& requester, PingingStats& stats)
{
	const auto nRestPings = options.nPingCount - stats.nSent;
	const auto nDesiredProbeCount = nRestPings * 4;
	const auto nRequestedProbeCount = nDesiredProbeCount > 10 ? nDesiredProbeCount : 10;

	const string sUrl = OSSFMT("StartPingTestByCountry?countrycode=" << sCountryCode
		<< "&destination=" << sTarget
		<< "&probeslimit=" << nRequestedProbeCount
		<< "&timeout=" << options.nMaxTimeoutMs);

	ProbeApiRequester::Request request(sUrl);
	request.nHttpTimeoutSec += options.nMaxTimeoutMs / 1000;

	const ProbeApiRequester::Reply reply = requester.DoRequest(request, options.bDebug);
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

	for (const auto& info : items)
	{
		const bool bFirstIteration = (0 == stats.nSent);

		if (!bFirstIteration)
		{
			cout << flush;
			const int nMaxDelay = 500;
			const int nDelayMs = info.bTimeout ? 500 : info.nTimeMs;
			std::this_thread::sleep_for(std::chrono::milliseconds((min)(nDelayMs, nMaxDelay)));
		}

		++stats.nSent;
		if (info.bTimeout)
		{
			cout << "Request timed out.";
		}
		else
		{
			++stats.nReceived;
			stats.nPingMin = (min)(stats.nPingMin, info.nTimeMs);
			stats.nPingMax = (max)(stats.nPingMax, info.nTimeMs);
			stats.nPingSum += info.nTimeMs;
			cout << "Reply from " << sTarget << ": bytes=" << PROBEAPI_PING_PACKET_SIZE << " time=" << info.nTimeMs << "ms TTL=" << PROBEAPI_PING_TTL;
		}

		if (options.bVerbose)
		{
			cout << " to " << info.sUniqueId << " (" << info.network.sName << ")";
		}

		cout << endl;
	}

	return eRetCode::OK;
}

//------------------------------------------------------

int PingByCountry(const ProgramOptions& options)
{
	int res = eRetCode::OK;

	ProbeApiRequester requester;

	const string& sTarget = options.sTarget;

	cout << endl << "Pinging " << sTarget << " with " << PROBEAPI_PING_PACKET_SIZE << " bytes of data" << flush;

	string sCountryCode = options.sModeArgument;
	if (DEFAULT_PING_COUNTRY_META == sCountryCode)
	{
		sCountryCode = GetDefaultCountryToPing(requester, options);
	}

	cout << " from country code " << sCountryCode << ":" << endl << flush;

	PingingStats stats;

	while (stats.nSent < options.nPingCount)
	{
		const auto nPreviousSend = stats.nSent;

		const int nRes = MakePackOfPingsByCountry(sCountryCode, sTarget, options, requester, stats);
		if (eRetCode::OK != nRes)
		{
			res = nRes;
			break;
		}
		if (nPreviousSend == stats.nSent)
		{
			// don't try again if no results are returned!
			break;
		}
	}

	cout << endl << "Ping statistics for " << sTarget << endl;
	cout << "    Packets : Sent = " << stats.nSent << ", Received = " << stats.nReceived << ", Lost = " << (stats.nSent - stats.nReceived)
		<< " (" << ((stats.nSent - stats.nReceived) * 100 / (stats.nSent ? stats.nSent : 1)) << " % loss)," << endl;

	if (stats.nReceived > 0)
	{
		cout << "Approximate round trip times in milli-seconds:" << endl;
		cout << "    Minimum = " << stats.nPingMin << "ms, Maximum = " << stats.nPingMax << "ms, Average = " << (stats.nPingSum / stats.nReceived) << "ms" << endl;
	}

	return res;
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

	const ProbeApiRequester::Reply reply = requester.DoRequest(request, options.bDebug);
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
		if (a.sName != b.sName)
		{
			return a.sName < b.sName;
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

		cout << setw(2) << info.sCode << " " << setw(40) << left << info.sName << right << " " << setw(5) << info.nProbes << endl;
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
