//------------------------------------------------------
#include "stdafx.h"
#include "Common.h"

#include "config.h"					// for MASHAPE_API_URL, MASHAPE_API_ID
#include "version.h"				// for VERSION_PRODUCT_NAME, MAIN_PRODUCT_VERSION_STR_A

using namespace std;

//------------------------------------------------------

ProbeApiRequest::ProbeApiRequest(const std::string& sRequestWithArgs)
{
	eMethod = HTTP_GET;
	sUrl = MASHAPE_API_URL + sRequestWithArgs;
	headers.push_back(make_pair("X-Mashape-Key", MASHAPE_API_ID));
	headers.push_back(make_pair("Accept", "application/json"));

	sUserAgent = VERSION_PRODUCT_NAME " HTTP client v." MAIN_PRODUCT_VERSION_STR_A;
	nHttpTimeoutSec = 30;
	bKnownBadSslCertificate = false;
}

//------------------------------------------------------






//------------------------------------------------------
