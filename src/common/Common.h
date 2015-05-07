//------------------------------------------------------
#ifndef _COMMON_H_UID000003467CD53C58
#define _COMMON_H_UID000003467CD53C58

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "HttpRequest.h"

//------------------------------------------------------

class ProbeApiRequest : public HttpRequester::Request
{
public:
	ProbeApiRequest(const std::string& sRequestWithArgs);
};

//------------------------------------------------------
#endif //ifndef _COMMON_H_UID000003467CD53C58
