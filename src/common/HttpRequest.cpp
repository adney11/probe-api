//------------------------------------------------------
#include "stdafx.h"
#include "HttpRequest.h"

#include "Common.h"

#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/Exception.hpp>
#include <curlpp/Infos.hpp>

//------------------------------------------------------

#ifdef _MSC_VER
#pragma comment (lib, "libcurl.lib")
#pragma comment (lib, "curlpp.lib")
// libcurl DLL dependencies:
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Wldap32.lib")
#endif // _MSC_VER > 1000

//------------------------------------------------------

using namespace std;

//------------------------------------------------------

volatile bool HttpRequester::bPauseAllRequests = false;
volatile bool HttpRequester::bTerminateAllRequests = false;

//------------------------------------------------------

HttpRequester::HttpRequester()
{
	// Note! curlpp documentation gives wrong info about how this function is implemented:
	curlpp::initialize();
}

//------------------------------------------------------

HttpRequester::~HttpRequester()
{
	try
	{
		// Note! curlpp documentation gives wrong info about how this function is implemented:
		curlpp::terminate();
	}
	catch (curlpp::LogicError& e)
	{
		cerr << "LogicError EXCEPTION was catched in " << __FUNCTION__ << ": " << e.what() << endl;
	}
	catch (curlpp::RuntimeError& e)
	{
		cerr << "RuntimeError EXCEPTION was catched in " << __FUNCTION__ << ": " << e.what() << endl;
	}
}

//------------------------------------------------------

void PrepareHttpRequest(curlpp::Easy &req, const HttpRequester::Request &info)
{
	list<string> headers;
	for (auto p : info.headers)
	{
		headers.push_back(p.first + ": " + p.second);
	}

	using namespace curlpp::Options;

	req.setOpt(new Url(info.sUrl));

	switch (info.eMethod)
	{
	case HttpRequester::Request::HTTP_GET:
		//req.setOpt(new CustomRequest("GET"));
		req.setOpt(new HttpGet(true));
		break;
// 	case HttpRequester::Request::HTTP_POST:
// 		//req.setOpt(new CustomRequest("POST"));
// 		req.setOpt(new Post(true));
// 		break;
	}

	if (info.bKnownBadSslCertificate)
	{
		req.setOpt(new SslVerifyHost(false));	// SslVerifyHost does not support "1" value
	}

	if (!info.sUserAgent.empty())
	{
		req.setOpt(new UserAgent(info.sUserAgent));
	}
	if (!info.sReferer.empty())
	{
		req.setOpt(new Referer(info.sReferer));
	}

	if (!headers.empty())
	{
		req.setOpt(new HttpHeader(headers));
	}

	//req.setOpt(new PostFields(info.sBody));
	//req.setOpt(new PostFieldSize(info.sBody.length()));

	req.setOpt(new Timeout(info.nHttpTimeoutSec));
	req.setOpt(new ConnectTimeout(info.nHttpTimeoutSec));

	req.setOpt(new FollowLocation(true));
}

//------------------------------------------------------

string GetDebugPrefix(const curl_infotype type)
{
	switch (type)
	{
	case CURLINFO_TEXT:
		return "t: ";
	case CURLINFO_HEADER_IN:
		return "h< ";
	case CURLINFO_DATA_IN:
		return "d< ";
	case CURLINFO_SSL_DATA_IN:
		return "s< ";
	case CURLINFO_HEADER_OUT:
		return "h> ";
	case CURLINFO_DATA_OUT:
		return "d> ";
	case CURLINFO_SSL_DATA_OUT:
		return "s> ";
	case CURLINFO_END:
		return "e: ";
	}
	return "z: ";
}

//------------------------------------------------------

HttpRequester::Reply HttpRequester::DoRequest(const HttpRequester::Request& info, const bool bVerbose)
{
	HttpRequester::Reply reply;
	reply.bSucceeded = false;

	try
	{
		curlpp::Easy req;

		using namespace curlpp::Options;

		if (bVerbose)
		{
			cout << "HttpRequester verbose mode ON!" << endl;
		}
		else
		{
			//cout << "HttpRequester verbose mode OFF!" << endl;
		}
		req.setOpt(new Verbose(bVerbose));
#if 1
		req.setOpt(DebugFunction([](const curl_infotype type, const char *data, const size_t size) -> int
		{
			while (bPauseAllRequests)
				MySleep(1);
			if (bTerminateAllRequests)
				return 0;

			string sData(data, size);
#ifdef DEST_OS_WINDOWS
			// Replace "\r\n" to "\n" because "\n" is replaced into "\r\n" in Windows automatically
			findandreplace(sData, "\r\n", "\n");
#endif
			cout << GetDebugPrefix(type) << sData;
			return size;
		}));
#endif

#if 1
		req.setOpt(WriteFunction([&reply](const char* ptr, const size_t size, const size_t nmemb) -> int
		{
			while (bPauseAllRequests)
				MySleep(1);
			if (bTerminateAllRequests)
				return 0;

			const size_t realsize = size * nmemb;
			reply.sBody.append(ptr, realsize);
			return realsize;
		}));
#endif

#if 1
		req.setOpt(ProgressFunction([&reply](const double dltotal, const double dlnow, const double ultotal, const double ulnow) -> int
		{
			while (bPauseAllRequests)
				MySleep(1);
			if (bTerminateAllRequests)
				return 1;
			return 0;
		}));
#endif

		PrepareHttpRequest(req, info);

		while (bPauseAllRequests)
			MySleep(1);
		if (bTerminateAllRequests)
			throw PException("DoRequest: Terminate all requests");

		req.perform();

		reply.bSucceeded = true;
		reply.nHttpCode = curlpp::infos::ResponseCode::get(req);
		reply.sContentType = curlpp::infos::ContentType::get(req);
		reply.sEffectiveUrl = curlpp::infos::EffectiveUrl::get(req);
	}
	catch (curlpp::LogicError& e)
	{
		reply.sErrorDescription = string("LogicError: ") + e.what();
	}
	catch (curlpp::RuntimeError& e)
	{
		reply.sErrorDescription = string("RuntimeError: ") + e.what();
	}

	return reply;
}

//------------------------------------------------------
