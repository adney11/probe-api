//------------------------------------------------------
#include "stdafx.h"
#include "dping.h"

#include "common/Common.h"
#include "common/CommonOperations.h"
#include "common/ParseReply.h"

using namespace std;

#ifdef DEST_OS_WINDOWS
#define PRINT_AS_WINDOWS
#endif

//------------------------------------------------------

class JobType;
shared_ptr<JobType> g_ptrJob = nullptr;
shared_ptr<ApplicationStats> g_ptrStats = nullptr;

//------------------------------------------------------
// Windows sample: (Win 8.1)

// c:\bin>ping 8.8.8.8
// 
// Pinging 8.8.8.8 with 32 bytes of data:
// Reply from 8.8.8.8: bytes=32 time=14ms TTL=55
// Reply from 8.8.8.8: bytes=32 time=13ms TTL=55
// Reply from 8.8.8.8: bytes=32 time=13ms TTL=55
// Reply from 8.8.8.8: bytes=32 time=13ms TTL=55
// 
// Ping statistics for 8.8.8.8:
// Packets: Sent = 4, Received = 4, Lost = 0 (0% loss),
// Approximate round trip times in milli-seconds:
// Minimum = 13ms, Maximum = 14ms, Average = 13ms
// 
// c:\bin>

//------------------------------------------------------
// Linux sample: (ubuntu)

// sergey@ubuntu:~$ ping 8.8.8.8
// PING 8.8.8.8 (8.8.8.8) 56(84) bytes of data.
// 64 bytes from 8.8.8.8: icmp_seq=1 ttl=128 time=13.7 ms
// 64 bytes from 8.8.8.8: icmp_seq=2 ttl=128 time=14.0 ms
// 64 bytes from 8.8.8.8: icmp_seq=3 ttl=128 time=15.7 ms
// 64 bytes from 8.8.8.8: icmp_seq=4 ttl=128 time=15.4 ms
// 64 bytes from 8.8.8.8: icmp_seq=5 ttl=128 time=15.4 ms
// 64 bytes from 8.8.8.8: icmp_seq=6 ttl=128 time=13.5 ms
// 64 bytes from 8.8.8.8: icmp_seq=7 ttl=128 time=14.4 ms
// 64 bytes from 8.8.8.8: icmp_seq=8 ttl=128 time=14.9 ms
// 64 bytes from 8.8.8.8: icmp_seq=9 ttl=128 time=14.3 ms
// 64 bytes from 8.8.8.8: icmp_seq=10 ttl=128 time=13.8 ms
// 64 bytes from 8.8.8.8: icmp_seq=11 ttl=128 time=18.1 ms
// 64 bytes from 8.8.8.8: icmp_seq=12 ttl=128 time=14.6 ms
// 64 bytes from 8.8.8.8: icmp_seq=13 ttl=128 time=16.4 ms
// ^C
// --- 8.8.8.8 ping statistics ---
// 13 packets transmitted, 13 received, 0% packet loss, time 12025ms
// rtt min/avg/max/mdev = 13.521/14.979/18.114/1.232 ms
// sergey@ubuntu:~$

// sergey@ubuntu:~$ ping 1.1.1.2
// PING 1.1.1.2 (1.1.1.2) 56(84) bytes of data.
// ^C
// --- 1.1.1.2 ping statistics ---
// 33 packets transmitted, 0 received, 100% packet loss, time 32059ms
// 
// sergey@ubuntu:~$

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
	~JobType()
	{
	}

	string GetUrl(const ApplicationStats& stats, const string& sSearchArgument, const string& sTarget) const
	{
		// Documentation:
		// https://www.mashape.com/optimalsoftware/freeprobeapi/#startpingtestbycountry
		// https://www.mashape.com/optimalsoftware/freeprobeapi/#startpingtestbyasn
		const int64_t nRestProbes = options.nProbesLimit - stats.nSent;
		const auto nRequestedProbeCount = (std::min)(options.nMaxProbesToRequest, (std::max)(options.nMinProbesToRequest, nRestProbes));

		const string sUrl = OSSFMT(sMethod
			<< "?" << sSearchArgName << "=" << sSearchArgument
			<< "&destination=" << sTarget					// An IP address or hostname that will be pinged
			<< "&probeslimit=" << nRequestedProbeCount		// Number of probes to use
			<< "&count=" << options.nPingsPerProbe			// Number of pings to run (default 3). (per each probe)
			<< "&ttl=" << options.nTTL						// Max number of hops for ping
			<< "&sleep=" << options.nWaitBetweenPingsMs		// Sleep between pings in milliseconds (default 1000ms).
			<< "&timeout=" << options.nTimeoutTotalMs		// Maximum time available to probes for testing in milliseconds (default 6000). The whole test is most likely to last longer then this value.
			<< "&bufferSize=" << options.nPacketSize		// buffer size filled with 'A' char to send, default=32, max=65500
			<< "&fragment=" << !options.bDontFragment		// opposite of "dontFragment" flag, default=1 => dontFragment=0
			<< "&resolve=" << options.bResolveIp2Name		// if IP was given, try to resolve it, default=0
			<< "&ipv4only=" << options.bUseIpv4Only			// force using IPv4 (if no IPv4 IP address is returned, return error), default=0
			<< "&ipv6only=" << options.bUseIpv6Only			// force using IPv6 (if no IPv6 IP address is returned, return error), default=0
			);

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
			const CommonOptions options2 = options.GetCommonOptions();
			sSearchArgument = GetDefaultSourceCountry(requester, options2);
		}
		return sSearchArgument;
	}

	void PrintHeaderBeforeSearchArg() const
	{
#ifdef PRINT_AS_WINDOWS
		// 
		// Pinging 8.8.8.8 with 32 bytes of data:
		// Pinging google-public-dns-a.google.com [8.8.8.8] with 32 bytes of data:
		cout << endl;
		cout << "Pinging " << options.sTarget << " with " << options.nPacketSize << " bytes of data";
#else
		// PING 8.8.8.8 (8.8.8.8) 56(84) bytes of data.
		// PING ya.ru (213.180.193.3) 56(84) bytes of data.
		const int nHeadersSize = 20 + 8; // IP + ICMP headers size
		cout << "PING " << options.sTarget << " (" << options.sTarget << ") " << options.nPacketSize << "(" << (options.nPacketSize + nHeadersSize) << ") bytes of data";
#endif
	}

	void PrintHeaderAfterSearchArg(const string& sSearchArgument) const
	{
#ifdef PRINT_AS_WINDOWS
		cout << " from " << FormatSearchDetails(sSearchArgument) << ":" << endl;
#else
		cout << " from " << FormatSearchDetails(sSearchArgument) << "." << endl;
#endif
	}

	void PrintJobResult(const ProbeAPI::ProbeInfo& info, const ProbeAPI::PingResult& pingResult) const
	{
#ifdef PRINT_AS_WINDOWS
		// Reply from 8.8.8.8: bytes=32 time=13ms TTL=55
		if (pingResult.bTimeout)
		{
			cout << "Request timed out.";
		}
		else
		{
			const auto& remote = info.ping;
			const string sTargetInfo = options.sTarget == remote.sTargetHost ? remote.sTargetIp : remote.sTargetHost + " [" + remote.sTargetIp + "]";

			cout << "Reply from " << sTargetInfo << ": bytes=" << options.nPacketSize << " time=" << pingResult.nTimeMs << "ms TTL=" << options.nTTL;
		}

		if (options.bVerbose)
		{
			cout << " for " << info.GetProbeInfo(options.mode == ApplicationOptions::MODE_DO_BY_ASN);
		}

		cout << endl;
#else
		// 64 bytes from 8.8.8.8: icmp_seq=1 ttl=128 time=13.7 ms
		if (pingResult.bTimeout)
		{
		}
		else
		{
			const auto& remote = info.ping;
			const string sTargetInfo = options.sTarget == remote.sTargetHost ? remote.sTargetIp : remote.sTargetHost + " (" + remote.sTargetIp + ")";

			cout << options.nPacketSize << " bytes from " << sTargetInfo << ": icmp_seq=1 ttl=" << options.nTTL << " time=" << pingResult.nTimeMs << ".0 ms";
			if (options.bVerbose)
			{
				cout << " for " << info.GetProbeInfo(options.mode == ApplicationOptions::MODE_DO_BY_ASN);
			}
			cout << endl;
		}
#endif
	}

	void PrintFooter(const ApplicationStats& stats) const
	{
#ifdef PRINT_AS_WINDOWS
		// 
		// Ping statistics for 8.8.8.8:
		// Packets: Sent = 4, Received = 4, Lost = 0 (0% loss),
		// Approximate round trip times in milli-seconds:
		// Minimum = 13ms, Maximum = 14ms, Average = 13ms
		cout << endl;
		cout << "Ping statistics for " << options.sTarget << endl;
		cout << "    Packets : Sent = " << stats.nSent << ", Received = " << stats.nReceived << ", Lost = " << (stats.nSent - stats.nReceived)
			<< " (" << ((stats.nSent - stats.nReceived) * 100 / (stats.nSent ? stats.nSent : 1)) << "% loss)," << endl;

		if (stats.nReceived > 0)
		{
			cout << "Approximate round trip times in milli-seconds:" << endl;
			cout << "    Minimum = " << stats.pings.GetMin() << "ms, Maximum = "
				<< stats.pings.GetMax() << "ms, Average = " << (int)stats.pings.GetAverage() << "ms" << endl;
		}
#else
		// ^C
		// --- 8.8.8.8 ping statistics ---
		// 13 packets transmitted, 13 received, 0% packet loss, time 12025ms
		// rtt min/avg/max/mdev = 13.521/14.979/18.114/1.232 ms
		cout << "--- " << options.sTarget << " ping statistics ---" << endl;
		cout << stats.nSent << " packets transmitted, " << stats.nReceived << " received, "
			<< ((stats.nSent - stats.nReceived) * 100 / (stats.nSent ? stats.nSent : 1)) << "% packet loss, time "
			<< stats.GetTimeElapsedMs() << "ms" << endl;

		if (stats.nReceived > 0)
		{
			cout << "rtt min/avg/max/mdev = " << fixed << setprecision(3)
				<< (double)stats.pings.GetMin() << "/" << (double)stats.pings.GetAverage()
				<< "/" << (double)stats.pings.GetMax() << "/" << (double)stats.pings.GetMedianAbsoluteDeviation()
				<< resetiosflags(cout.flags()) << resetiosflags(ios_base::floatfield) << " ms" << endl;
		}
#endif
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
	shared_ptr<ApplicationStats> ptrStats = g_ptrStats;
	shared_ptr<JobType> ptrJob = g_ptrJob;

	if (ptrJob && ptrStats)
	{
		ptrJob->PrintFooter(*ptrStats);
	}
}

//------------------------------------------------------

void PrintPackOfResults(const JobType& job, const ApplicationOptions& options, const vector<ProbeAPI::ProbeInfo>& items, ApplicationStats& stats)
{
	//bool bFirstIteration = (0 == stats.nSent);
	bool bFirstIteration = true;
	for (const auto& info : items)
	{
		for (const auto& pingResult : info.ping.vectResults)
		{
			if (g_bTerminateProgram)
				throw PException("PrintPackOfResults: Terminate Program");

			++stats.nSent;
			if (!pingResult.bTimeout)
			{
				++stats.nReceived;
				stats.pings.AddItem(pingResult.nTimeMs);
			}

			if (!options.bNoDelays)
			{
				DoSleep(pingResult, bFirstIteration);
			}
			job.PrintJobResult(info, pingResult);
		}
	}
}

//------------------------------------------------------

int MakePackOfJobs(const JobType& job, const string& sSearchArgument,
	const ApplicationOptions& options, ProbeApiRequester& requester, ApplicationStats& stats)
{
	const string sUrl = job.GetUrl(stats, sSearchArgument, options.sTarget);

	ProbeApiRequester::Request request(sUrl, options.GetCommonOptions());
	request.nHttpTimeoutSec += options.nTimeoutTotalMs / 1000;

	const ProbeApiRequester::Reply reply = requester.DoRequest(request, options.bDebug);
	if (!reply.bSucceeded)
	{
		throw PException("MakePackOfJobs: requester.DoRequest: " + reply.sErrorDescription, eRetCode::ApiFailure);
	}

	vector<ProbeAPI::ProbeInfo> items;

	try
	{
		items = ProbeAPI::ParsePingResults(reply.sBody, job.GetResultRoot());
	}
	catch (PException& e)
	{
		throw PException("MakePackOfJobs: ProbeAPI::ParsePingResults: " + e.str(), eRetCode::ApiParsingFail);
	}

	PrintPackOfResults(job, options, items, stats);

	return eRetCode::OK;
}

//------------------------------------------------------

int DoJob(const ApplicationOptions& options)
{
	int res = eRetCode::OK;

	shared_ptr<ApplicationStats> ptrStats = make_shared<ApplicationStats>(options.sTarget);
	ApplicationStats& stats = *ptrStats;
	g_ptrStats = ptrStats;

	shared_ptr<JobType> ptrJob = make_shared<JobType>(options);
	const JobType& job = *ptrJob;
	g_ptrJob = ptrJob;

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
		g_ptrStats.reset();
		g_ptrJob.reset();
		throw;
	}

	job.PrintFooter(stats);
	g_ptrStats.reset();
	g_ptrJob.reset();

	return res;
}

//------------------------------------------------------

int Application(const ApplicationOptions& options)
{
	const CommonOptions options2 = options.GetCommonOptions();

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
