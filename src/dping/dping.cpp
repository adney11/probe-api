//------------------------------------------------------
#include "stdafx.h"
#include "dping.h"

#include "common/Common.h"
#include "common/CommonOperations.h"
#include "common/ParseReply.h"

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

class JobType
{
public:
	JobType(const ApplicationOptions& options_) : options(options_)
	{
		switch (options.mode)
		{
		case ApplicationOptions::MODE_DO_BY_COUNTRY:
			sMethod = "StartPingTestByCountry";
			sSearchArgName = "countrycode";
			sResultRoot = "StartPingTestByCountryResult";
			sSearchDetails = "country code {ARG}";
			break;
		case ApplicationOptions::MODE_DO_BY_ASN:
			sMethod = "StartPingTestByAsn";
			sSearchArgName = "asnid";
			sResultRoot = "StartPingTestByASNResult";
			sSearchDetails = "{ARG}";
			break;
		default:
			sMethod = "does_not_exist";
			sSearchArgName = "does_not_exist";
			sResultRoot = "does_not_exist";
			sSearchDetails = "{ARG}";
			break;
		}
	}

	string GetUrl(const ApplicationStats& stats, const string& sSearchArgument, const string& sTarget) const
	{
		const auto nRestJobs = options.nCount - stats.nSent;
		const auto nDesiredProbeCount = nRestJobs * 4;
		const auto nRequestedProbeCount = nDesiredProbeCount > 10 ? nDesiredProbeCount : 10;

		const string sUrl = OSSFMT(sMethod
			<< "?" << sSearchArgName << "=" << sSearchArgument
			<< "&destination=" << sTarget
			<< "&probeslimit=" << nRequestedProbeCount
			<< "&timeout=" << options.nMaxTimeoutMs);

		return sUrl;
	}

	string GetResultRoot() const
	{
		return sResultRoot;
	}

	string FormatSearchDetails(const string& sSearchArgument) const
	{
		return findandreplaceConst(sSearchDetails, "{ARG}", sSearchArgument);
	}

	string CalculateSearchArgument(ProbeApiRequester& requester) const
	{
		string sSearchArgument = options.sModeArgument;
		if (ApplicationOptions::MODE_DO_BY_COUNTRY == options.mode && DEFAULT_COUNTRY_META == sSearchArgument)
		{
			const CommonOptions options2(options.bDebug, options.sModeArgument);
			sSearchArgument = GetDefaultSourceCountry(requester, options2);
		}
		return sSearchArgument;
	}

protected:
	const ApplicationOptions&	options;
	string						sMethod;
	string						sSearchArgName;
	string						sSearchDetails;
	string						sResultRoot;
};

//------------------------------------------------------

void PrintPackOfResults(const string& sTarget, const ApplicationOptions& options, const vector<ProbeAPI::ProbeInfo>& items, ApplicationStats& stats)
{
	bool bFirstIteration = (0 == stats.nSent);
	for (const auto& info : items)
	{
		if (g_bTerminateProgram)
			throw PException("PrintPackOfResults: Terminate Program");

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
}

//------------------------------------------------------

int MakePackOfJobs(const JobType& job, const string& sSearchArgument, const string& sTarget,
	const ApplicationOptions& options, ProbeApiRequester& requester, ApplicationStats& stats)
{
	const string sUrl = job.GetUrl(stats, sSearchArgument, sTarget);

	ProbeApiRequester::Request request(sUrl);
	request.nHttpTimeoutSec += options.nMaxTimeoutMs / 1000;

	const ProbeApiRequester::Reply reply = requester.DoRequest(request, options.bDebug);
	if (!reply.bSucceeded)
	{
		throw PException("MakePackOfJobs: " + reply.sErrorDescription, eRetCode::ApiFailure);
	}

	vector<ProbeAPI::ProbeInfo> items;

	try
	{
		items = ProbeAPI::ParsePingResults(reply.sBody, job.GetResultRoot());
	}
	catch (PException& e)
	{
		throw PException("MakePackOfJobs: " + e.str(), eRetCode::ApiParsingFail);
	}

	PrintPackOfResults(sTarget, options, items, stats);

	return eRetCode::OK;
}

//------------------------------------------------------

int DoJob(const ApplicationOptions& options)
{
	int res = eRetCode::OK;

	const string& sTarget = options.sTarget;

	cout << endl;
	cout << "Pinging " << sTarget << " with " << options.nPacketSize << " bytes of data";
	cout << flush;

	const JobType job(options);
	ProbeApiRequester requester;

	const string sSearchArgument = job.CalculateSearchArgument(requester);

	cout << " from " << job.FormatSearchDetails(sSearchArgument) << ":" << endl;
	cout << flush;

	ApplicationStats stats(sTarget);

	try
	{
		while (stats.nSent < options.nCount)
		{
			const auto nPreviousSend = stats.nSent;

			const int nRes = MakePackOfJobs(job, sSearchArgument, sTarget, options, requester, stats);
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
		return DoJob(options);
	case ApplicationOptions::MODE_DO_BY_ASN:
		return DoJob(options);
	case ApplicationOptions::MODE_GET_COUNTRIES:
		return ListCountries(options2);
	case ApplicationOptions::MODE_GET_ASNS:
		return ListAsns(options2);
	default:
		throw PException(eRetCode::NotSupported) << "Unknown program mode " << options.mode;
	}
}

//------------------------------------------------------
