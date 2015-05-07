//------------------------------------------------------
#ifndef _HTTPREQUEST_H_UID000003466406882C
#define _HTTPREQUEST_H_UID000003466406882C

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>
#include <list>

//------------------------------------------------------

class HttpRequester
{
public:
	HttpRequester();
	~HttpRequester();

public:
	struct Request
	{
		enum HTTP_METHOD
		{
			HTTP_GET,
			//HTTP_POST,
		};
		HTTP_METHOD		eMethod;
		std::string		sUrl;
		//std::string		sBody;

		std::list<std::pair<std::string, std::string>> headers;
		std::string		sUserAgent;
		std::string		sReferer;

		int				nHttpTimeoutSec;
		bool			bKnownBadSslCertificate;

		Request() : eMethod(HTTP_GET), nHttpTimeoutSec(5*60), bKnownBadSslCertificate(false)
		{}
	};

	struct Reply
	{
		bool			bSucceeded;
		std::string		sErrorDescription;

		int				nHttpCode;
		std::string		sEffectiveUrl;
		std::string		sContentType;

		std::string		sBody;

		Reply() : bSucceeded(false), nHttpCode(0)
		{}
	};

public:
	Reply DoRequest(const Request& requestInfo, const bool bVerbose);
};

//------------------------------------------------------
#endif //ifndef _HTTPREQUEST_H_UID000003466406882C
