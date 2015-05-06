//------------------------------------------------------

#include "stdafx.h"

#include "common\Common.h"
#include "common\HttpRequest.h"
#include "common\ParseReply.h"
#include "common\version.h"

#include <iostream>

//------------------------------------------------------

#if _MSC_VER > 1000
#pragma comment (lib, "common.lib")
#endif // _MSC_VER > 1000

//------------------------------------------------------

using namespace std;

//------------------------------------------------------

int main(int argc, char* argv[])
{
	cout << "ProbeAPI dping v." MAIN_PRODUCT_VERSION_STR_A  << endl;

	HttpRequester requester;

	HttpRequester::Request request;
	request.sUrl = "https://google.com/";
	request.sUserAgent = "ProbeAPI http client v." MAIN_PRODUCT_VERSION_STR_A;
	request.nHttpTimeoutSec = 20;
	HttpRequester::Reply reply = requester.DoRequest(request);

	return 0;
}

//------------------------------------------------------
