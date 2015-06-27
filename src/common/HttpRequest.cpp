//------------------------------------------------------
#include "stdafx.h"
#include "HttpRequest.h"

#include "Common.h"

#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/Exception.hpp>
#include <curlpp/Infos.hpp>

#include <regex>

//------------------------------------------------------

#ifdef _MSC_VER
//#pragma comment (lib, "libcurl.lib")
//#pragma comment (lib, "curlpp.lib")
// libcurl DLL dependencies:
//#pragma comment (lib, "Ws2_32.lib")
//#pragma comment (lib, "Wldap32.lib")
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
	req.setOpt(new SslVerifyPeer(false));

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

		ostream& ostr = cout; // cout, cerr
		ostream& ostrMore = cerr; // cout, cerr

		if (bVerbose)
		{
			ostr << "HttpRequester verbose mode ON!" << endl;
		}
		else
		{
			//cout << "HttpRequester verbose mode OFF!" << endl;
		}
		req.setOpt(new Verbose(bVerbose));
#if 1
		req.setOpt(DebugFunction([&ostrMore](const curl_infotype type, const char *data, const size_t size) -> int
		{
			string sData(data, size);

			// Replace "\r\n" to "\n" because "\n" is replaced into "\r\n" in Windows automatically
			// And we don't need those "\r" in linux console too:
			findandreplace(sData, "\r\n", "\n");

			ostrMore << GetDebugPrefix(type) << sData << flush;
			return 0;
		}));
#endif

#if 1
		req.setOpt(WriteFunction([&reply](const char* ptr, const size_t size, const size_t nmemb) -> int
		{
			const size_t realsize = size * nmemb;
			reply.sBody.append(ptr, realsize);
			return realsize;
		}));
#endif

#if 1
		req.setOpt(NoProgress(false));
		req.setOpt(ProgressFunction([](const double dltotal, const double dlnow, const double ultotal, const double ulnow) -> int
		{
			while (bPauseAllRequests)
				MySleep(1);
			if (bTerminateAllRequests)
				return 1;
			return 0;
		}));
#endif

		string sHttpStatusText;

		// Get HTTP headers to parse HTTP status text later:
		req.setOpt(HeaderFunction([&sHttpStatusText](char *buffer, size_t size, size_t nitems) -> size_t
		{
			string str(buffer, size * nitems);

			if (begins(str, "HTTP/"))
			{
				std::smatch sm;
				// HTTP/1.0 404 Not Found
				std::regex e(R"(^HTTP\/(?:1\.0|1\.1) \d{1,3} (.+))");
				if (std::regex_search(str, sm, e))
				{
					// Replace HTTP status text with text from latest responce:
					sHttpStatusText = sm[1];
				}
			}

			return size * nitems;
		}));

		PrepareHttpRequest(req, info);

		while (bPauseAllRequests)
			MySleep(1);
		if (bTerminateAllRequests)
			throw PException("DoRequest: Terminate all requests");

		cerr << flush;
		cout << flush;

		try
		{
			req.perform();
			reply.bSucceeded = true;
		}
		catch (curlpp::LibcurlRuntimeError& e)
		{
			switch (e.whatCode())
			{
			case CURLE_HTTP_RETURNED_ERROR:
			case CURLE_ABORTED_BY_CALLBACK:
				// in case of CURLE_HTTP_RETURNED_ERROR or CURLE_ABORTED_BY_CALLBACK we can safely continue:
				reply.sErrorDescription = "Error code " + to_string(e.whatCode()) + ": " + curl_easy_strerror(e.whatCode()) + "; " + e.what();
				break;
			default:
				throw;
			}
		}

		reply.nHttpCode = curlpp::infos::ResponseCode::get(req);
		reply.sHttpStatusText = sHttpStatusText;
		reply.sEffectiveUrl = curlpp::infos::EffectiveUrl::get(req);

		// Unfortunately libcurl can return NULL value for CURLINFO_EFFECTIVE_URL and 
		// currently curlpp does not support this and makes a crash.
		// So here I used manual way to get content type:
		//reply.sContentType = curlpp::infos::ContentType::get(req);
		{
			char * tmp = nullptr;
			curlpp::InfoGetter::get<char*>(req, CURLINFO_CONTENT_TYPE, tmp);
			if (tmp)
			{
				reply.sContentType = tmp;
			}
			else
			{
				reply.sContentType = "";
			}
		}
	}
	catch (curlpp::LibcurlLogicError& e)
	{
		reply.sErrorDescription = string("cURL LibcurlLogicError: ")
			+ "code " + to_string(e.whatCode()) + ": " + curl_easy_strerror(e.whatCode()) + "; " + e.what();
	}
	catch (curlpp::LibcurlRuntimeError& e)
	{
		reply.sErrorDescription = string("cURL LibcurlRuntimeError: ")
			+ "code " + to_string(e.whatCode()) + ": " + curl_easy_strerror(e.whatCode()) + "; " + e.what();
	}
	catch (curlpp::LogicError& e)
	{
		reply.sErrorDescription = string("cURL LogicError: ") + e.what();
	}
	catch (curlpp::RuntimeError& e)
	{
		reply.sErrorDescription = string("cURL RuntimeError: ") + e.what();
	}

	return reply;
}

//------------------------------------------------------
