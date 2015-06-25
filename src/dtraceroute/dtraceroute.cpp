//------------------------------------------------------
#include "stdafx.h"
#include "dtraceroute.h"

#include "common/Common.h"
#include "common/CommonOperations.h"
#include "common/ParseReply.h"

using namespace std;

#ifdef DEST_OS_WINDOWS
#define PRINT_AS_WINDOWS
#endif

//------------------------------------------------------

class JobType;
JobType* g_pJob = nullptr;
ApplicationStats* g_pStats = nullptr;

//------------------------------------------------------
// Windows sample: (Win 8.1)

// c:\bin>tracert 8.8.8.8
// 
// Tracing route to google-public-dns-a.google.com [8.8.8.8]
// over a maximum of 30 hops:
// 
//   1    <1 ms    <1 ms    <1 ms  10.168.0.1
//   2     3 ms     3 ms     3 ms  134.17.128.1
//   3     2 ms     2 ms     1 ms  172.30.65.36
//   4     2 ms     2 ms     2 ms  172.30.65.25
//   5    43 ms     2 ms     2 ms  185.32.224.156
//   6    13 ms    12 ms    12 ms  188.254.103.221
//   7    28 ms    15 ms    12 ms  95.167.95.222
//   8    47 ms    44 ms    44 ms  79.133.94.86
//   9    13 ms    13 ms    14 ms  216.239.47.143
//  10    13 ms    13 ms    13 ms  google-public-dns-a.google.com [8.8.8.8]
// 
// Trace complete.
// 
// c:\bin>

//------------------------------------------------------
// Linux sample: (ubuntu)

// sergey@ubuntu:$ traceroute 8.8.8.8
// traceroute to 8.8.8.8 (8.8.8.8), 30 hops max, 60 byte packets
//  1  192.168.163.2 (192.168.163.2)  0.160 ms  0.109 ms  0.068 ms
//  2  * * *
//  3  * * *
// ...
// 29  * * *
// 30  * * *
// sergey@ubuntu:$

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
		g_pJob = this;
	}
	~JobType()
	{
		g_pJob = nullptr;
	}

	string GetUrl(const ApplicationStats& stats, const string& sSearchArgument, const string& sTarget) const
	{
		// Documentation:
		// https://www.mashape.com/optimalsoftware/freeprobeapi/#starttracerttestbycountry
		// https://www.mashape.com/optimalsoftware/freeprobeapi/#starttracerttestbyasn
		const auto nRestProbes = options.nProbesLimit - stats.nSent;
		const auto nDesiredProbeCount = nRestProbes; // nRestProbes * 2;
		const auto nRequestedProbeCount = nDesiredProbeCount > 10 ? nDesiredProbeCount : 10;

		const string sUrl = OSSFMT(sMethod
			<< "?" << sSearchArgName << "=" << sSearchArgument
			<< "&destination=" << sTarget					// Comma-separated list of IP addresses or hostnames that will be tested
			<< "&probeslimit=" << nRequestedProbeCount		// Number of probes to use
			<< "&count=" << options.nTriesPerHop			// Number of pings to each hop. Default: 3
			<< "&ttl=" << options.nMaxHops					// Max number of hops.
			<< "&sleep=" << options.nWaitBetweenPingsMs		// Sleep between pings in milliseconds (default 300ms).
			<< "&maxFailedHops=" << options.nMaxHopsFailed	// max number of rows where all pings got timeout result
			<< "&commandTimeout=" << options.nTimeoutPingMs	// Ping timeout in milliseconds that one ping can take (default 5000ms).
			<< "&timeout=" << options.nTimeoutTotalMs);		// Maximum time available to probes for testing in milliseconds (default 6000). The whole test is most likely to last longer then this value.

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
			const CommonOptions options2(options.bDebug, options.bVerbose, options.sModeArgument, options.nResultsLimit);
			sSearchArgument = GetDefaultSourceCountry(requester, options2);
		}
		return sSearchArgument;
	}

	void PrintHeaderBeforeSearchArg() const
	{
#ifdef PRINT_AS_WINDOWS
		// Tracing route to 1.1.1.2 over a maximum of 30 hops
		// 
		// Tracing route to google-public-dns-a.google.com [8.8.8.8]
		// over a maximum of 30 hops:
		// 
		cout << endl;
		cout << "Tracing route to " << options.sTarget;
#else
		// traceroute to 8.8.8.8 (8.8.8.8), 30 hops max, 60 byte packets
		cout << "traceroute to " << options.sTarget << " (" << options.sTarget << ")";
#endif
	}

	void PrintHeaderAfterSearchArg(const string& sSearchArgument) const
	{
#ifdef PRINT_AS_WINDOWS
		// Tracing route to 1.1.1.2 over a maximum of 30 hops
		// 
		// Tracing route to google-public-dns-a.google.com [8.8.8.8]
		// over a maximum of 30 hops:
		// 
		cout << " from " << FormatSearchDetails(sSearchArgument) << ":" << endl;
		cout << "over a maximum of " << options.nMaxHops << " hops:" << endl;
		cout << endl;
#else
		// traceroute to 8.8.8.8 (8.8.8.8), 30 hops max, 60 byte packets
		cout << " from " << FormatSearchDetails(sSearchArgument) << ", " << options.nMaxHops << " hops max, " << options.nPacketSize << " byte packets" << endl;
#endif
	}

	void PrintJobStart(const ProbeAPI::ProbeInfo& info) const
	{
#ifdef PRINT_AS_WINDOWS
		// Tracing route to google-public-dns-a.google.com [8.8.8.8]
		// over a maximum of 30 hops:
		//if (options.bVerbose)
		{
			cout << "Tracing route to " << info.tracert.sTarget << " from " << info.GetPeerInfo(options.mode == ApplicationOptions::MODE_DO_BY_ASN) << endl;
			cout << "over a maximum of " << options.nMaxHops << " hops:" << endl;
		}
#else
		// Tracing route to google-public-dns-a.google.com [8.8.8.8]
		// over a maximum of 30 hops:
		//if (options.bVerbose)
		{
			cout << "traceroute to " << options.sTarget << " (" << options.sTarget << ")";
			cout << " from " << info.GetPeerInfo(options.mode == ApplicationOptions::MODE_DO_BY_ASN) << ", " << options.nMaxHops << " hops max, " << options.nPacketSize << " byte packets" << endl;
		}
#endif
	}

	void PrintHopStart(const int nHop, const ProbeAPI::TracertHopResults& hop) const
	{
#ifdef PRINT_AS_WINDOWS
		//   1    <1 ms    <1 ms    <1 ms  10.10.0.1
		//   2     3 ms     3 ms     4 ms  124.47.118.1
		//   3     *        *        *     124.47.118.100
		cout << setw(3) << nHop;
#else
		//  1  192.168.163.2 (192.168.163.2)  0.160 ms  0.109 ms  0.068 ms
		//  2  * * *
		cout << setw(2) << nHop << "  " << hop.sReplyHost << " (" << hop.sReplyHost << ")";
#endif
	}

	void PrintHopTry(const ProbeAPI::PingResult& ping) const
	{
#ifdef PRINT_AS_WINDOWS
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
#else
		if (ping.bTimeout)
		{
			cout << " *";
		}
		else
		{
			cout << "  " << ping.nTimeMs << ".000 ms";
		}
#endif
	}

	void PrintHopFinish(const ProbeAPI::TracertHopResults& hop) const
	{
#ifdef PRINT_AS_WINDOWS
		cout << "  " << hop.sReplyHost << endl;
#else
		cout << endl;
#endif
	}

	void PrintJobFinish() const
	{
#ifdef PRINT_AS_WINDOWS
		cout << endl;
		cout << "Trace complete." << endl;
		cout << endl;
#else
#endif
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

void PrintFinalStats()
{
	if (g_pJob && g_pStats)
	{
		g_pJob->PrintFooter(*g_pStats);
	}
}

//------------------------------------------------------

void PrintPackOfResults(const JobType& job, const ApplicationOptions& options, const vector<ProbeAPI::ProbeInfo>& items, ApplicationStats& stats)
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

			job.PrintHopStart(++iHop, hop);

			for (const auto& ping : hop.vectResults)
			{
				if (g_bTerminateProgram)
					throw PException("PrintPackOfResults: loop3: Terminate Program");

				if (!options.bNoDelays)
				{
					DoSleep(info.ping, bFirstIteration);
				}
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
	request.nHttpTimeoutSec += options.nTimeoutTotalMs / 1000;

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

	PrintPackOfResults(job, options, items, stats);

	// hack to have only one call to this function:
	stats.nSent = options.nProbesLimit;

	return eRetCode::OK;
}

//------------------------------------------------------

int DoJob(const ApplicationOptions& options)
{
	int res = eRetCode::OK;

	ApplicationStats stats(options.sTarget);

	const JobType job(options);
	job.PrintHeaderBeforeSearchArg();

	ProbeApiRequester requester;
	const string sSearchArgument = job.CalculateSearchArgument(requester);

	job.PrintHeaderAfterSearchArg(sSearchArgument);

	try
	{
		while (stats.nSent < options.nProbesLimit)
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
	const CommonOptions options2(options.bDebug, options.bVerbose, options.sModeArgument, options.nResultsLimit);

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
