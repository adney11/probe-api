//------------------------------------------------------
#include "stdafx.h"
#include "HttpRequest.h"

#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/Exception.hpp>
#include <curlpp/Infos.hpp>

//------------------------------------------------------

#if _MSC_VER > 1000
#pragma comment (lib, "libcurl.lib")
#pragma comment (lib, "curlpp.lib")
// libcurl DLL dependencies:
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Wldap32.lib")
#endif // _MSC_VER > 1000

//------------------------------------------------------

using namespace std;

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

HttpRequester::Reply HttpRequester::DoRequest(const HttpRequester::Request& info, const bool bVerbose)
{
	HttpRequester::Reply reply;
	reply.bSucceeded = false;

	try
	{
		curlpp::Easy req;

		using namespace curlpp::Options;

		req.setOpt(new Verbose(bVerbose));
#if 0
		req.setOpt(DebugFunction([](curl_infotype type, char *data, size_t size) -> int
		{
			//curlpp::raiseException(runtime_error("EXCEPTION: " + string(data, size)));
			cerr << "DEBUG(" << (int)type << "): " << string(data, size);
			return size;
		}));
#endif

#if 1
		req.setOpt(WriteFunction([&reply](char* ptr, size_t size, size_t nmemb) -> int
		{
			const size_t realsize = size * nmemb;
			reply.sBody.append(ptr, realsize);
			return realsize;
		}));
#endif

		PrepareHttpRequest(req, info);

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
