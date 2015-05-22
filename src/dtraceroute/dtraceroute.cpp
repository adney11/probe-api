//------------------------------------------------------
#include "stdafx.h"
#include "dtraceroute.h"

#include "common/Common.h"
#include "common/CommonOperations.h"
#include "common/ParseReply.h"

using namespace std;

//------------------------------------------------------

class JobType
{
public:
	JobType(const ApplicationOptions& options_) : options(options_)
	{
		switch (options.mode)
		{
		case ApplicationOptions::MODE_DO_BY_COUNTRY:
			sMethod = "StartTracertTestByCountry";
			sSearchArgName = "countrycode";
			sResultRoot = "StartTracertTestByCountryResult";
			sSearchDetails = "country code {ARG}";
			break;
		case ApplicationOptions::MODE_DO_BY_ASN:
			sMethod = "StartTracertTestByAsn";
			sSearchArgName = "asnid";
			sResultRoot = "StartTracertTestByASNResult";
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
			<< "&ttl=" << options.nTTL
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

	void PrintHeaderBeforeSearchArg() const
	{
		// Tracing route to 1.1.1.2 over a maximum of 30 hops
		cout << endl;
		cout << "Tracing route to " << options.sTarget;
	}

	void PrintHeaderAfterSearchArg(const string& sSearchArgument) const
	{
		cout << " from " << FormatSearchDetails(sSearchArgument) << ":" << endl;
		cout << "over a maximum of " << options.nTTL << " hops:" << endl;
		cout << endl;
	}

	void PrintJobStart(const ProbeAPI::ProbeInfo& info) const
	{
		// Tracing route to google-public-dns-a.google.com [8.8.8.8]
		// over a maximum of 30 hops:
		if (options.bVerbose)
		{
			cout << "Tracing route to [" << info.tracert.sTarget << "] from " << info.GetPeerInfo(options.mode == ApplicationOptions::MODE_DO_BY_ASN) << endl;
			cout << "over a maximum of " << options.nTTL << " hops:" << endl;
		}
	}

	void PrintHopStart(const int nHop) const
	{
		//   1    <1 ms    <1 ms    <1 ms  10.10.0.1
		//   2     3 ms     3 ms     4 ms  124.47.118.1
		//   3     *        *        *     124.47.118.100
		cout << setw(3) << nHop;
	}

	void PrintHopTry(const ProbeAPI::PingResult& ping) const
	{
		const int nWidth = 5;
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

	void PrintHopFinish(const ProbeAPI::TracertHopResults& hop) const
	{
		cout << "  " << hop.sReplyHost << endl;
	}

	void PrintJobFinish() const
	{
		cout << endl;
		cout << "Trace complete." << endl;
		cout << endl;
	}

	void PrintFooter(const ApplicationStats& stats) const
	{
	}

protected:
	const ApplicationOptions&	options;
	string						sMethod;
	string						sSearchArgName;
	string						sSearchDetails;
	string						sResultRoot;
};

//------------------------------------------------------

void PrintPackOfResults(const JobType& job, const vector<ProbeAPI::ProbeInfo>& items, ApplicationStats& stats)
{
	bool bFirstIteration = (0 == stats.nSent);
	for (const auto& info : items)
	{
		if (g_bTerminateProgram)
			throw PException("PrintPackOfResults: loop1: Terminate Program");

		job.PrintJobStart(info);

		size_t iHop = 0;
		for (const auto& hop : info.tracert.vectHops)
		{
			if (g_bTerminateProgram)
				throw PException("PrintPackOfResults: loop2: Terminate Program");

			job.PrintHopStart(++iHop);

			for (const auto& ping : hop.vectResults)
			{
				if (g_bTerminateProgram)
					throw PException("PrintPackOfResults: loop3: Terminate Program");

				DoSleep(ping, bFirstIteration);
				job.PrintHopTry(ping);
			}

			job.PrintHopFinish(hop);
		}

		job.PrintJobFinish();
	}
}

//------------------------------------------------------

int MakePackOfJobs(const JobType& job, const string& sSearchArgument,
	const ApplicationOptions& options, ProbeApiRequester& requester, ApplicationStats& stats)
{
	const string sUrl = job.GetUrl(stats, sSearchArgument, options.sTarget);

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
		items = ProbeAPI::ParseTracertResults(reply.sBody, job.GetResultRoot());
	}
	catch (PException& e)
	{
		throw PException("MakePackOfJobs: " + e.str(), eRetCode::ApiParsingFail);
	}

	PrintPackOfResults(job, items, stats);

	// hack to have only one call to this function:
	stats.nSent = options.nCount;

	return eRetCode::OK;
}

//------------------------------------------------------

int DoJob(const ApplicationOptions& options)
{
	int res = eRetCode::OK;

	const JobType job(options);
	job.PrintHeaderBeforeSearchArg();

	ProbeApiRequester requester;
	const string sSearchArgument = job.CalculateSearchArgument(requester);

	job.PrintHeaderAfterSearchArg(sSearchArgument);

	ApplicationStats stats(options.sTarget);

	try
	{
		while (stats.nSent < options.nCount)
		{
			const auto nPreviousSend = stats.nSent;

			const int nRes = MakePackOfJobs(job, sSearchArgument, options, requester, stats);
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

			if (g_bTerminateProgram)
				throw PException("DoJob: Terminate Program");
		}
	}
	catch (...)
	{
		job.PrintFooter(stats);
		throw;
	}

	job.PrintFooter(stats);

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
