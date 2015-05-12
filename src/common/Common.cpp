//------------------------------------------------------
#include "stdafx.h"
#include "Common.h"

#include "config.h"					// for MASHAPE_API_URL, MASHAPE_API_ID
#include "version.h"				// for VERSION_PRODUCT_NAME, MAIN_PRODUCT_VERSION_STR_A

#include <json/json.h>

//------------------------------------------------------

#ifdef _MSC_VER
#pragma comment (lib, "jsoncpp.lib")
#endif // _MSC_VER > 1000

//------------------------------------------------------

using namespace std;

//------------------------------------------------------

ProbeApiRequester::Request::Request(const std::string& sRequestWithArgs)
{
	eMethod = HTTP_GET;
	sUrl = MASHAPE_API_URL + sRequestWithArgs;
	headers.emplace_back("X-Mashape-Key", MASHAPE_API_ID);
	headers.emplace_back("Accept", "application/json");

	sUserAgent = VERSION_PRODUCT_NAME " HTTP client v." MAIN_PRODUCT_VERSION_STR_A;
	nHttpTimeoutSec = 60;
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
