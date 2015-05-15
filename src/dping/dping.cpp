//------------------------------------------------------
#include "stdafx.h"
#include "dping.h"

#include "common/config.h"					// for PROBEAPI_PING_PACKET_SIZE, PROBEAPI_PING_TTL
#include "common/Common.h"
#include "common/ParseReply.h"

#include <json/json.h>

#include <algorithm>
#include <functional>
#include <thread>
#include <chrono>

using namespace std;

//------------------------------------------------------

PingingStats*	g_pPingStats = nullptr;

//------------------------------------------------------

PingingStats::PingingStats(const string& sTarget_) : sTarget(sTarget_)
{
	g_pPingStats = this;
}

//------------------------------------------------------

PingingStats::~PingingStats()
{
	g_pPingStats = nullptr;
}

//------------------------------------------------------

void PingingStats::Print()
{
	cout << endl << "Ping statistics for " << sTarget << endl;
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
	std::vector<ProbeInfo> items;

	try
	{
		items = ParsePingTestByCountryResult(reply.sBody);
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

	const string& sTarget = options.sTarget;

	cout << endl << "Pinging " << sTarget << " with " << PROBEAPI_PING_PACKET_SIZE << " bytes of data";
	cout << flush;

	ProbeApiRequester requester;

	string sCountryCode = options.sModeArgument;
	if (DEFAULT_PING_COUNTRY_META == sCountryCode)
	{
		sCountryCode = GetDefaultCountryToPing(requester, options);
	}

	cout << " from country code " << sCountryCode << ":" << endl;
	cout << flush;

	PingingStats stats(sTarget);

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

	stats.Print();

	return res;
}

//------------------------------------------------------

int MakePackOfPingsByAsn(const string& sAsnId, const string& sTarget, const ProgramOptions& options, ProbeApiRequester& requester, PingingStats& stats)
{
	const auto nRestPings = options.nPingCount - stats.nSent;
	const auto nDesiredProbeCount = nRestPings * 4;
	const auto nRequestedProbeCount = nDesiredProbeCount > 10 ? nDesiredProbeCount : 10;

	// Note! Currently StartPingTestByAsn does not support timeout argument, but I kept it here just in case.
	const string sUrl = OSSFMT("StartPingTestByAsn?asnid=" << sAsnId
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
	std::vector<ProbeInfo> items;

	try
	{
		items = ParsePingTestByAsnResult(reply.sBody);
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

int PingByAsn(const ProgramOptions& options)
{
	int res = eRetCode::OK;

	const string& sTarget = options.sTarget;
	const string& sAsnId = options.sModeArgument;

	cout << endl << "Pinging " << sTarget << " with " << PROBEAPI_PING_PACKET_SIZE << " bytes of data";
	cout << " from " << sAsnId << ":" << endl;
	cout << flush;

	ProbeApiRequester requester;

	PingingStats stats(sTarget);

	while (stats.nSent < options.nPingCount)
	{
		const auto nPreviousSend = stats.nSent;

		const int nRes = MakePackOfPingsByAsn(sAsnId, sTarget, options, requester, stats);
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

	stats.Print();

	return res;
}

//------------------------------------------------------

int ListCountries(const ProgramOptions& options)
{
	const int nWidth1 = 2;
	const int nWidth2 = 40;
	const int nWidth3 = 5;
	cout << setw(nWidth1) << "ID" << " " << setw(nWidth2) << left << "Country Name" << " " << setw(nWidth3) << "Number of hosts" << endl;
	cout << flush;

	ProbeApiRequester requester;

	ProbeApiRequester::Request request("GetCountries");

	const ProbeApiRequester::Reply reply = requester.DoRequest(request, options.bDebug);
	if (!reply.bSucceeded)
	{
		cerr << "ERROR! " << reply.sErrorDescription << endl;
		return eRetCode::ApiFailure;
	}

	cout << setfill('-') << setw(nWidth1 + 1 + nWidth2 + 1 + nWidth3 + 10) << "-" << setfill(' ') << endl;
	cout << flush;

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

	for (const auto& info : items)
	{
		if (0 == info.nProbes)
		{
			//continue;
		}

		cout << setw(nWidth1) << info.sCode << " " << setw(nWidth2) << left << info.sName << right << " " << setw(nWidth3) << info.nProbes << endl;
	}

	return eRetCode::OK;
}

//------------------------------------------------------

struct MyAsnInfo
{
	ProbeAPI::ProbeInfo		probe;
	int						nCount = 0;
};

//------------------------------------------------------

int ListAsns(const ProgramOptions& options)
{
	const string sCountryCode = options.sModeArgument;

	const int nWidth1 = 8;
	const int nWidth2 = 6;
	cout << setw(nWidth1) << left << "ASN id" << right << " " << setw(nWidth2) << "Probes" << " " << "ASN name" << endl;
	cout << flush;

	ProbeApiRequester requester;

	const string sUrl = OSSFMT("GetProbesByCountry?countrycode=" << sCountryCode);

	ProbeApiRequester::Request request(sUrl);

	const ProbeApiRequester::Reply reply = requester.DoRequest(request, options.bDebug);
	if (!reply.bSucceeded)
	{
		cerr << "ERROR! " << reply.sErrorDescription << endl;
		return eRetCode::ApiFailure;
	}

	cout << setfill('-') << setw(nWidth1 + 1 + nWidth2 + 1 + 40) << "-" << setfill(' ') << endl;
	cout << flush;

	using namespace ProbeAPI;
	std::vector<ProbeInfo> items;

	try
	{
		items = ParseGetProbesByCountryResult_AsnOnly(reply.sBody);
	}
	catch (exception& e)
	{
		cerr << "ERROR! " << e.what() << endl;
		return eRetCode::ApiFailure;
	}

	typedef int32_t AsnId;
	map<AsnId, MyAsnInfo> items2;

	for (const auto& info : items)
	{
		AsnId id = 0;
		// Usually ASN id has a form "AS202732" where constant prefix "AS" is followed by integer:
		if (begins(info.asn.sId, "AS"))
		{
			id = std::stoul(info.asn.sId.substr(2));
		}
		else
		{
			hash<string> h1;
			id = h1(info.asn.sId);
		}

		auto& item = items2[id];
		item.probe = info;
		++item.nCount;
	}

	for (const auto& p : items2)
	{
		const auto& info = p.second;
		cout << setw(nWidth1) << left << info.probe.asn.sId << right << " " << setw(nWidth2) << info.nCount << " " << info.probe.asn.sName << endl;
	}

	return eRetCode::OK;
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
