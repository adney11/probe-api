//------------------------------------------------------
#ifndef _COMMON_H_UID000003467CD53C58
#define _COMMON_H_UID000003467CD53C58

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "HttpRequest.h"

//------------------------------------------------------

class ProbeApiRequester: protected HttpRequester
{
public:
	struct Request : HttpRequester::Request
	{
		Request(const std::string& sRequestWithArgs);
	};

	typedef HttpRequester::Reply Reply;

public:
	Reply DoRequest(const Request& requestInfo, const bool bVerbose);
};

//------------------------------------------------------
// error codes returned by the process:

namespace eRetCode
{
	enum enumRetCode: int
	{
		NoValue			= -1,
		OK				= 0,
		BadArguments	= 10,
		NotSupported	= 20,
		ApiFailure		= 100,
		OtherError		= 500,
	};
}

//------------------------------------------------------

inline bool contains(const std::string& str, const std::string& substr)
{
	return str.find(substr) != str.npos;
}

inline bool begins(const std::string& s, const std::string& begin)
{
	const auto len = begin.length();
	if (s.length() < len)
		return false;

	return 0 == memcmp(s.c_str(), begin.c_str(), len);
}

inline bool ends(const std::string& s, const std::string& end)
{
	const auto len = end.length();
	if (s.length() < len)
		return false;

	return 0 == memcmp(s.c_str() + s.length() - len, end.c_str(), len);
}

//------------------------------------------------------
#endif //ifndef _COMMON_H_UID000003467CD53C58
