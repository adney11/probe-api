//------------------------------------------------------
#include "stdafx.h"
#include "dping.h"

#include "common\Common.h"

#include <iostream>

using namespace std;

//------------------------------------------------------

int Dping(const ProgramOptions& options)
{
	HttpRequester requester;

	ProbeApiRequest request("GetCountries");
	//request.sUrl = "https://google.com/";
	HttpRequester::Reply reply = requester.DoRequest(request, options.bVerbose);

	cout << "request succeeded: " << reply.bSucceeded << endl;
	cout << "request error desc: " << reply.sErrorDescription << endl;
	cout << "reply HTTP code: " << reply.nHttpCode << endl;
	cout << "reply EffectiveUrl: " << reply.sEffectiveUrl << endl;
	cout << "reply ContentType: " << reply.sContentType << endl;
	cout << "reply body length: " << reply.sBody.length() << endl;

	//cout << "REPLY BODY: [[[" << reply.sBody << "]]]" << endl;

	return 0;
}

//------------------------------------------------------
