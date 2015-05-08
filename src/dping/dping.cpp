//------------------------------------------------------
#include "stdafx.h"
#include "dping.h"

#include "common\Common.h"

#include <iostream>

using namespace std;

//------------------------------------------------------

void HttpReplyDebugPrint(const ProbeApiRequester::Reply &reply)
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

int PingByCountry(const ProgramOptions& options)
{
	cerr << "ERROR! This program mode is not implemented yet!" << endl;
	return 2;
}

//------------------------------------------------------

int PingByAsn(const ProgramOptions& options)
{
	cerr << "ERROR! This program mode is not implemented yet!" << endl;
	return 2;
}

//------------------------------------------------------

int ListCountries(const ProgramOptions& options)
{
	ProbeApiRequester requester;

	ProbeApiRequester::Request request("GetCountries");
	//request.sUrl = "https://google.com/";
	const ProbeApiRequester::Reply reply = requester.DoRequest(request, options.bVerbose);

	HttpReplyDebugPrint(reply);

	return reply.bSucceeded ? 0 : 2;
}

//------------------------------------------------------

int ListAsns(const ProgramOptions& options)
{
	cerr << "ERROR! This program mode is not implemented yet!" << endl;
	return 2;
}

//------------------------------------------------------

int Dping(const ProgramOptions& options)
{
	switch (options.mode)
	{
	case ProgramOptions::MODE_PING_BY_COUNTRY:
		return PingByCountry(options);
	case ProgramOptions::MODE_PING_BY_ASN:
		return PingByAsn(options);
	case ProgramOptions::MODE_GET_COUNTRIES:
		return ListCountries(options);
	case ProgramOptions::MODE_GET_ASNS:
		return ListAsns(options);
	default:
		cerr << "ERROR! Unknown program mode " << options.mode << endl;
		return 1;
	}
}

//------------------------------------------------------
