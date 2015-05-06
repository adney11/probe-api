//------------------------------------------------------
#include "stdafx.h"
#include "HttpRequest.h"

#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/Exception.hpp>

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
		cerr << "EXCEPTION inside of " << __FUNCTION__ << ": " << e.what() << endl;
	}
	catch (curlpp::RuntimeError& e)
	{
		cerr << "EXCEPTION inside of " << __FUNCTION__ << ": " << e.what() << endl;
	}
}

//------------------------------------------------------

HttpRequester::Reply HttpRequester::DoRequest(const HttpRequester::Request& info)
{
	HttpRequester::Reply reply;
	reply.bSucceeded = false;

	try
	{
		curlpp::Easy req;

		list<string> headers;
		for (auto p : info.headers)
		{
			headers.push_back(p.first + ": " + p.second);
		}

		using namespace curlpp::Options;

		req.setOpt(new Verbose(true));
#if 1
		req.setOpt(DebugFunction([](curl_infotype, char *data, size_t size) -> int
		{
			//curlpp::raiseException(runtime_error("EXCEPTION: " + string(data, size)));
			cerr << "DEBUG: " << string(data, size);
			return size;
		}));
#endif

		//------------------------------------------------------

		req.setOpt(new Url(info.sUrl));

		switch (info.eMethod)
		{
		case Request::HTTP_GET:
			//req.setOpt(new CustomRequest("GET"));
			req.setOpt(new HttpGet(true));
			break;
// 		case Request::HTTP_POST:
// 			//req.setOpt(new CustomRequest("POST"));
// 			req.setOpt(new Post(true));
// 			break;
		}

		if (info.bKnownBadSslCertificate)
		{
			req.setOpt(new SslVerifyHost(false));
		}

		req.setOpt(new UserAgent(info.sUserAgent));
		req.setOpt(new Referer(info.sReferer));

		req.setOpt(new HttpHeader(headers));

		//req.setOpt(new PostFields(info.sBody));
		//req.setOpt(new PostFieldSize(info.sBody.length()));

		req.setOpt(new Timeout(info.nHttpTimeoutSec));
		req.setOpt(new ConnectTimeout(info.nHttpTimeoutSec));

		req.setOpt(new FollowLocation(true));

		req.perform();

	}
	catch (curlpp::LogicError& e)
	{
		cerr << "EXCEPTION inside of " << __FUNCTION__ << ": " << e.what() << endl;
	}
	catch (curlpp::RuntimeError& e)
	{
		cerr << "EXCEPTION inside of " << __FUNCTION__ << ": " << e.what() << endl;
	}

	return reply;
}

//------------------------------------------------------
