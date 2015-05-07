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
	HttpRequester::Reply reply = requester.DoRequest(request);

	return 0;
}

//------------------------------------------------------
