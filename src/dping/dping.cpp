//------------------------------------------------------
#include "stdafx.h"
#include "dping.h"

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
			const CommonOptions options2(options.bDebug, options.bVerbose, options.sModeArgument, options.nCount);
			sSearchArgument = GetDefaultSourceCountry(requester, options2);
		}
		return sSearchArgument;
	}

	void PrintHeaderBeforeSearchArg() const
	{
		// Pinging 8.8.8.8 with 32 bytes of data:
		cout << endl;
		cout << "Pinging " << options.sTarget << " with " << options.nPacketSize << " bytes of data";
	}

	void PrintHeaderAfterSearchArg(const string& sSearchArgument) const
	{
		cout << " from " << FormatSearchDetails(sSearchArgument) << ":" << endl;
	}

	void PrintJobResult(const ProbeAPI::ProbeInfo& info) const
	{
		// Reply from 8.8.8.8: bytes=32 time=13ms TTL=55
		if (info.ping.bTimeout)
		{
			cout << "Request timed out.";
		}
		else
		{
			cout << "Reply from " << options.sTarget << ": bytes=" << options.nPacketSize << " time=" << info.ping.nTimeMs << "ms TTL=" << options.nTTL;
		}

		if (options.bVerbose)
		{
			cout << " to " << info.GetPeerInfo(options.mode == ApplicationOptions::MODE_DO_BY_ASN);
		}

		cout << endl;
	}

	void PrintFooter(const ApplicationStats& stats) const
	{
		cout << endl;
		cout << "Ping statistics for " << options.sTarget << endl;
		cout << "    Packets : Sent = " << stats.nSent << ", Received = " << stats.nReceived << ", Lost = " << (stats.nSent - stats.nReceived)
			<< " (" << ((stats.nSent - stats.nReceived) * 100 / (stats.nSent ? stats.nSent : 1)) << " % loss)," << endl;

		if (stats.nReceived > 0)
		{
			cout << "Approximate round trip times in milli-seconds:" << endl;
			cout << "    Minimum = " << stats.nPingMin << "ms, Maximum = " << stats.nPingMax << "ms, Average = " << (stats.nPingSum / stats.nReceived) << "ms" << endl;
		}
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
			throw PException("PrintPackOfResults: Terminate Program");

		++stats.nSent;
		if (!info.ping.bTimeout)
		{
			++stats.nReceived;
			stats.nPingMin = (min)(stats.nPingMin, info.ping.nTimeMs);
			stats.nPingMax = (max)(stats.nPingMax, info.ping.nTimeMs);
			stats.nPingSum += info.ping.nTimeMs;
		}

		DoSleep(info.ping, bFirstIteration);
		job.PrintJobResult(info);
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
		items = ProbeAPI::ParsePingResults(reply.sBody, job.GetResultRoot());
	}
	catch (PException& e)
	{
		throw PException("MakePackOfJobs: " + e.str(), eRetCode::ApiParsingFail);
	}

	PrintPackOfResults(job, items, stats);

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
	const CommonOptions options2(options.bDebug, options.bVerbose, options.sModeArgument, options.nCount);

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
