//------------------------------------------------------
#include "stdafx.h"
#include "dtraceroute.h"

#include "common/Common.h"
#include "common/CommonOperations.h"
#include "common/ParseReply.h"

#include <json/json.h>

#include <algorithm>

using namespace std;

//------------------------------------------------------

ApplicationStats*	g_pApplicationStats = nullptr;

//------------------------------------------------------

ApplicationStats::ApplicationStats(const string& sTarget_) : sTarget(sTarget_)
{
	g_pApplicationStats = this;
}

//------------------------------------------------------

ApplicationStats::~ApplicationStats()
{
	g_pApplicationStats = nullptr;
}

//------------------------------------------------------

void ApplicationStats::Print()
{
}

//------------------------------------------------------

void PrintPackOfResults(const string& sTarget, const ApplicationOptions& options, const vector<ProbeAPI::ProbeInfo>& items, ApplicationStats& stats)
{
	bool bFirstIteration = (0 == stats.nSent);
	for (const auto& info : items)
	{
		if (g_bTerminateProgram)
			throw PException("PrintPackOfResults: loop1: Terminate Program");

		if (options.bVerbose)
		{
			cout << "Tracing route to [" << info.tracert.sTarget << "] from host " << info.sUniqueId << " (" << info.network.sName << ")" << endl;
			cout << "over a maximum of " << options.nTTL << " hops:" << endl;
		}

		size_t iHop = 0;
		for (const auto& hop : info.tracert.vectHops)
		{
			if (g_bTerminateProgram)
				throw PException("PrintPackOfResults: loop2: Terminate Program");

			// Tracing route to google-public-dns-a.google.com [8.8.8.8]
			// over a maximum of 30 hops:
			//   1    <1 ms    <1 ms    <1 ms  10.10.0.1
			//   2     3 ms     3 ms     4 ms  124.47.118.1
			//   3     *        *        *     124.47.118.100
			cout << setw(3) << ++iHop;
			for (const auto& ping : hop.vectResults)
			{
				if (g_bTerminateProgram)
					throw PException("PrintPackOfResults: loop3: Terminate Program");

				const int nWidth = 5;
				DoSleep(ping, bFirstIteration);

				if (ping.bTimeout)
				{
					cout << " " << setw(nWidth - 1) << " " << "*" << "   ";
				}
				else if (ping.nTimeMs < 1)
				{
					cout << " " << setw(nWidth - 2) << " " << "<1" << " ms";
				}
				else
				{
					cout << " " << setw(nWidth) << ping.nTimeMs << " ms";
				}
			}
			cout << "  " << hop.sReplyHost << endl;
		}

		cout << endl;
		cout << "Trace complete." << endl;
		cout << endl;
	}
}

//------------------------------------------------------

int MakePackOfJobsByCountry(const string& sCountryCode, const string& sTarget, const ApplicationOptions& options, ProbeApiRequester& requester, ApplicationStats& stats)
{
	const auto nRestJobs = options.nCount - stats.nSent;
	const auto nDesiredProbeCount = nRestJobs;
	const auto nRequestedProbeCount = nDesiredProbeCount > 10 ? nDesiredProbeCount : 10;

	const string sUrl = OSSFMT("StartTracertTestByCountry?countrycode=" << sCountryCode
		<< "&destination=" << sTarget
		<< "&probeslimit=" << nRequestedProbeCount
		<< "&ttl=" << options.nTTL
		<< "&timeout=" << options.nMaxTimeoutMs);

	ProbeApiRequester::Request request(sUrl);
	request.nHttpTimeoutSec += options.nMaxTimeoutMs / 1000;

	const ProbeApiRequester::Reply reply = requester.DoRequest(request, options.bDebug);
	if (!reply.bSucceeded)
	{
		throw PException("MakePackOfJobsByCountry: " + reply.sErrorDescription, eRetCode::ApiFailure);
	}

	vector<ProbeAPI::ProbeInfo> items;

	try
	{
		items = ProbeAPI::ParseTracertTestByCountryResult(reply.sBody);
	}
	catch (PException& e)
	{
		throw PException("MakePackOfJobsByCountry: " + e.str(), eRetCode::ApiParsingFail);
	}

	PrintPackOfResults(sTarget, options, items, stats);

	// hack to have only one call to this function:
	stats.nSent = options.nCount;

	return eRetCode::OK;
}

//------------------------------------------------------

int DoByCountry(const ApplicationOptions& options)
{
	int res = eRetCode::OK;

	const string& sTarget = options.sTarget;

	cout << endl;
	cout << "Tracing route to [" << sTarget << "]";
	cout << flush;

	ProbeApiRequester requester;

	string sCountryCode = options.sModeArgument;
	if (DEFAULT_COUNTRY_META == sCountryCode)
	{
		const CommonOptions options2(options.bDebug, options.sModeArgument);
		sCountryCode = GetDefaultSourceCountry(requester, options2);
	}

	cout << " from country code " << sCountryCode << ":" << endl;
	cout << "over a maximum of " << options.nTTL << " hops:" << endl;
	cout << endl;
	cout << flush;

	ApplicationStats stats(sTarget);

	try
	{
		while (stats.nSent < options.nCount)
		{
			const auto nPreviousSend = stats.nSent;

			const int nRes = MakePackOfJobsByCountry(sCountryCode, sTarget, options, requester, stats);
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
	}
	catch (...)
	{
		stats.Print();
		throw;
	}

	return res;
}

//------------------------------------------------------

int MakePackOfJobsByAsn(const string& sAsnId, const string& sTarget, const ApplicationOptions& options, ProbeApiRequester& requester, ApplicationStats& stats)
{
	const auto nRestJobs = options.nCount - stats.nSent;
	const auto nDesiredProbeCount = nRestJobs;
	const auto nRequestedProbeCount = nDesiredProbeCount > 10 ? nDesiredProbeCount : 10;

	const string sUrl = OSSFMT("StartTracertTestByAsn?asnid=" << sAsnId
		<< "&destination=" << sTarget
		<< "&probeslimit=" << nRequestedProbeCount
		<< "&ttl=" << options.nTTL
		<< "&timeout=" << options.nMaxTimeoutMs);

	ProbeApiRequester::Request request(sUrl);
	request.nHttpTimeoutSec += options.nMaxTimeoutMs / 1000;

	const ProbeApiRequester::Reply reply = requester.DoRequest(request, options.bDebug);
	if (!reply.bSucceeded)
	{
		throw PException("MakePackOfJobsByAsn: " + reply.sErrorDescription, eRetCode::ApiFailure);
	}

	vector<ProbeAPI::ProbeInfo> items;

	try
	{
		items = ProbeAPI::ParseTracertTestByAsnResult(reply.sBody);
	}
	catch (PException& e)
	{
		throw PException("MakePackOfJobsByAsn: " + e.str(), eRetCode::ApiParsingFail);
	}

	PrintPackOfResults(sTarget, options, items, stats);

	// hack to have only one call to this function:
	stats.nSent = options.nCount;

	return eRetCode::OK;
}

//------------------------------------------------------

int DoByAsn(const ApplicationOptions& options)
{
	int res = eRetCode::OK;

	const string& sTarget = options.sTarget;
	const string& sAsnId = options.sModeArgument;

	cout << endl;
	cout << "Tracing route to [" << sTarget << "]";
	cout << " from " << sAsnId << ":" << endl;
	cout << "over a maximum of " << options.nTTL << " hops:" << endl;
	cout << endl;
	cout << flush;

	ProbeApiRequester requester;

	ApplicationStats stats(sTarget);

	try
	{
		while (stats.nSent < options.nCount)
		{
			const auto nPreviousSend = stats.nSent;

			const int nRes = MakePackOfJobsByAsn(sAsnId, sTarget, options, requester, stats);
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
	}
	catch (...)
	{
		stats.Print();
		throw;
	}

	return res;
}

//------------------------------------------------------

int Application(const ApplicationOptions& options)
{
	const CommonOptions options2(options.bDebug, options.sModeArgument);

	switch (options.mode)
	{
	case ApplicationOptions::MODE_DO_BY_COUNTRY:
		return DoByCountry(options);
	case ApplicationOptions::MODE_DO_BY_ASN:
		return DoByAsn(options);
	case ApplicationOptions::MODE_GET_COUNTRIES:
		return ListCountries(options2);
	case ApplicationOptions::MODE_GET_ASNS:
		return ListAsns(options2);
	default:
		throw PException() << "Unknown program mode " << options.mode;
	}
}

//------------------------------------------------------
