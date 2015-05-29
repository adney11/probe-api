//------------------------------------------------------
#include "stdafx.h"
#include "Common.h"

#include "config.h"					// for MASHAPE_API_URL, MASHAPE_API_ID
#include "version.h"				// for VERSION_PRODUCT_NAME, MAIN_PRODUCT_VERSION_STR_A

#include <json/json.h>

#include <thread>

//------------------------------------------------------

#ifdef _MSC_VER
//#pragma comment (lib, "jsoncpp.lib")
#endif // _MSC_VER > 1000

//------------------------------------------------------

using namespace std;

//------------------------------------------------------

volatile bool g_bTerminateProgram = false;

//------------------------------------------------------

ProbeApiRequester::Request::Request(const std::string& sRequestWithArgs)
{
	eMethod = HTTP_GET;
	sUrl = MASHAPE_API_URL + sRequestWithArgs;
	headers.emplace_back("X-Mashape-Key", MASHAPE_API_ID);
	headers.emplace_back("Accept", "application/json");

	sUserAgent = VERSION_PRODUCT_NAME " HTTP client v." MAIN_PRODUCT_VERSION_STR_A;
	nHttpTimeoutSec = 2 * 60;
	bKnownBadSslCertificate = false;
}

//------------------------------------------------------

ProbeApiRequester::Reply ProbeApiRequester::DoRequest(const ProbeApiRequester::Request& requestInfo, const bool bVerbose)
{
	ProbeApiRequester::Reply reply = HttpRequester::DoRequest(requestInfo, bVerbose);

	// reply HTTP code: 401 Unauthorized
	// Content-Type: application/json
	// {"message":"Missing Mashape application key. Go to http:\/\/docs.mashape.com\/api-keys to learn how to get your API application key."}
	// 
	// reply HTTP code: 402
	// Content-Type: application/json
	// {"message":"You need to subscribe to a plan before consuming the API"}
	// 
	// reply HTTP code : 403
	// Content-Type: application/json
	// {"message":"Invalid Mashape Key"}
	// 
	// reply HTTP code: 404
	// Content-Type: text/html; charset=UTF-8
	// <body>
	// <div id="content">
	// <p class="heading1">Service</p>
	// <p>Endpoint not found.</p>
	// </div>
	// </body>

	const bool bJsonReply = begins(reply.sContentType, "application/json");
		
	if (reply.bSucceeded)
	{
		if (reply.nHttpCode != 200 && bJsonReply)
		{
			string sMessage;
			{
				Json::Reader reader;
				Json::Value root;
				const bool parsedOK = reader.parse(reply.sBody, root);
				if (parsedOK)
				{
					sMessage = root.get("message", "").asString();
				}
			}

			reply.bSucceeded = false;
			reply.sErrorDescription = OSSFMT("Bad HTTP reply code: " << reply.nHttpCode << "; message: " << sMessage);
		}
		else if (!bJsonReply)
		{
			reply.bSucceeded = false;
			reply.sErrorDescription = OSSFMT("Bad reply format. HTTP code: " << reply.nHttpCode << "; Content-Type: " << reply.sContentType);
		}
	}

	if (bVerbose)
	{
		HttpReplyDebugPrint(reply);
	}

	return reply;
}

//------------------------------------------------------

void ProbeApiRequester::HttpReplyDebugPrint(const ProbeApiRequester::Reply &reply)
{
	cout << "request succeeded: " << reply.bSucceeded << endl;
	cout << "request error desc: " << reply.sErrorDescription << endl;
	cout << "reply HTTP code: " << reply.nHttpCode << endl;
	cout << "reply EffectiveUrl: " << reply.sEffectiveUrl << endl;
	cout << "reply Content-Type: " << reply.sContentType << endl;
	cout << "reply body length: " << reply.sBody.length() << endl;

	cout << "REPLY BODY: [[[" << reply.sBody << "]]]" << endl;
}

//------------------------------------------------------

void MySleep(const uint32_t nSleepMs)
{
	this_thread::sleep_for(chrono::milliseconds(nSleepMs));
}

//------------------------------------------------------

template<class T>
inline T findandreplaceConstT(const T& source, const T& find, const T& replace)
{
	if (find.empty() || source.empty())
		return source;

	ptrdiff_t nPredictReplaces = 0;
	for (std::size_t pos = source.find(find); pos != source.npos; pos = source.find(find, pos))
	{
		++nPredictReplaces;
		pos += find.length();
	}

	T res;
	res.reserve(source.length() + nPredictReplaces * (replace.length() - find.length()));

	T::size_type posPrev = 0;
	for (std::size_t pos = source.find(find); pos != source.npos; pos = source.find(find, pos))
	{
		if (pos > posPrev)
		{
			res += source.substr(posPrev, pos - posPrev);
		}
		res += replace;
		pos += find.length();
		posPrev = pos;
	}

	if (posPrev != T::npos)
	{
		// Copy rest of the string:
		res += source.substr(posPrev);
	}

	return res;
}

//------------------------------------------------------

template<class T>
inline void findandreplaceT(T& source, const T& find, const T& replace)
{
	if (find.empty() || source.empty())
		return;

	// Optimize performance if find and replace have different size.
	// In this case we can't replace in-place without moving rest of string in memory.

	if (find.length() != replace.length())
	{
		std::swap(source, findandreplaceConstT(source, find, replace));
		return;
	}

	// Fast in-place string replacement:

	T::size_type pos = 0;
	while ((pos = source.find(find, pos)) != T::npos)
	{
		source.replace(pos, find.length(), replace);
		pos += replace.length();
	}
}

//------------------------------------------------------

std::string findandreplaceConst(const std::string& source, const std::string& find, const std::string& replace)
{
	return findandreplaceConstT(source, find, replace);
}

std::wstring findandreplaceConst(const std::wstring& source, const std::wstring& find, const std::wstring& replace)
{
	return findandreplaceConstT(source, find, replace);
}

//------------------------------------------------------

void findandreplace(std::string& source, const std::string& find, const std::string& replace)
{
	findandreplaceT(source, find, replace);
}

void findandreplace(std::wstring& source, const std::wstring& find, const std::wstring& replace)
{
	findandreplaceT(source, find, replace);
}

//------------------------------------------------------
