//------------------------------------------------------
#include "stdafx.h"
#include "dping.h"

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
	cout << endl;
	cout << "Ping statistics for " << sTarget << endl;
	cout << "    Packets : Sent = " << nSent << ", Received = " << nReceived << ", Lost = " << (nSent - nReceived)
		<< " (" << ((nSent - nReceived) * 100 / (nSent ? nSent : 1)) << " % loss)," << endl;

	if (nReceived > 0)
	{
		cout << "Approximate round trip times in milli-seconds:" << endl;
		cout << "    Minimum = " << nPingMin << "ms, Maximum = " << nPingMax << "ms, Average = " << (nPingSum / nReceived) << "ms" << endl;
	}
	cout << flush;
}

//------------------------------------------------------

int MakePackOfJobsByCountry(const string& sCountryCode, const string& sTarget, const ApplicationOptions& options, ProbeApiRequester& requester, ApplicationStats& stats)
{
	const auto nRestJobs = options.nCount - stats.nSent;
	const auto nDesiredProbeCount = nRestJobs * 4;
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
		throw PException("MakePackOfJobsByCountry: " + reply.sErrorDescription, eRetCode::ApiFailure);
	}

	using namespace ProbeAPI;
	vector<ProbeInfo> items;

	try
	{
		items = ParsePingTestByCountryResult(reply.sBody);
	}
	catch (PException& e)
	{
		throw PException("MakePackOfJobsByCountry: " + e.str(), eRetCode::ApiParsingFail);
	}

	bool bFirstIteration = true;
	for (const auto& info : items)
	{
		if (g_bTerminateProgram)
			throw PException("MakePackOfJobsByCountry: Terminate Program");

		// Pinging 8.8.8.8 with 32 bytes of data:
		// Reply from 8.8.8.8: bytes=32 time=13ms TTL=55
		DoSleep(info.ping, bFirstIteration);

		++stats.nSent;
		if (info.ping.bTimeout)
		{
			cout << "Request timed out.";
		}
		else
		{
			++stats.nReceived;
			stats.nPingMin = (min)(stats.nPingMin, info.ping.nTimeMs);
			stats.nPingMax = (max)(stats.nPingMax, info.ping.nTimeMs);
			stats.nPingSum += info.ping.nTimeMs;
			cout << "Reply from " << sTarget << ": bytes=" << options.nPacketSize << " time=" << info.ping.nTimeMs << "ms TTL=" << options.nTTL;
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

int DoByCountry(const ApplicationOptions& options)
{
	int res = eRetCode::OK;

	const string& sTarget = options.sTarget;

	cout << endl;
	cout << "Pinging " << sTarget << " with " << options.nPacketSize << " bytes of data";
	cout << flush;

	ProbeApiRequester requester;

	string sCountryCode = options.sModeArgument;
	if (DEFAULT_COUNTRY_META == sCountryCode)
	{
		const CommonOptions options2(options.bDebug, options.sModeArgument);
		sCountryCode = GetDefaultSourceCountry(requester, options2);
	}

	cout << " from country code " << sCountryCode << ":" << endl;
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
	const auto nDesiredProbeCount = nRestJobs * 4;
	const auto nRequestedProbeCount = nDesiredProbeCount > 10 ? nDesiredProbeCount : 10;

	const string sUrl = OSSFMT("StartPingTestByAsn?asnid=" << sAsnId
		<< "&destination=" << sTarget
		<< "&probeslimit=" << nRequestedProbeCount
		<< "&timeout=" << options.nMaxTimeoutMs);

	ProbeApiRequester::Request request(sUrl);
	request.nHttpTimeoutSec += options.nMaxTimeoutMs / 1000;

	const ProbeApiRequester::Reply reply = requester.DoRequest(request, options.bDebug);
	if (!reply.bSucceeded)
	{
		throw PException("MakePackOfJobsByAsn: " + reply.sErrorDescription, eRetCode::ApiFailure);
	}

	using namespace ProbeAPI;
	vector<ProbeInfo> items;

	try
	{
		items = ParsePingTestByAsnResult(reply.sBody);
	}
	catch (PException& e)
	{
		throw PException("MakePackOfJobsByAsn: " + e.str(), eRetCode::ApiParsingFail);
	}

	bool bFirstIteration = true;
	for (const auto& info : items)
	{
		if (g_bTerminateProgram)
			throw PException("MakePackOfJobsByAsn: Terminate Program");

		// Pinging 8.8.8.8 with 32 bytes of data:
		// Reply from 8.8.8.8: bytes=32 time=13ms TTL=55
		DoSleep(info.ping, bFirstIteration);

		++stats.nSent;
		if (info.ping.bTimeout)
		{
			cout << "Request timed out.";
		}
		else
		{
			++stats.nReceived;
			stats.nPingMin = (min)(stats.nPingMin, info.ping.nTimeMs);
			stats.nPingMax = (max)(stats.nPingMax, info.ping.nTimeMs);
			stats.nPingSum += info.ping.nTimeMs;
			cout << "Reply from " << sTarget << ": bytes=" << options.nPacketSize << " time=" << info.ping.nTimeMs << "ms TTL=" << options.nTTL;
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

int DoByAsn(const ApplicationOptions& options)
{
	int res = eRetCode::OK;

	const string& sTarget = options.sTarget;
	const string& sAsnId = options.sModeArgument;

	cout << endl;
	cout << "Pinging " << sTarget << " with " << options.nPacketSize << " bytes of data";
	cout << " from " << sAsnId << ":" << endl;
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
		cerr << "ERROR! Unknown program mode " << options.mode << endl;
		return eRetCode::NotSupported;
	}
}

//------------------------------------------------------
