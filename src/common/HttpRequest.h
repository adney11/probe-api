//------------------------------------------------------
#ifndef _HTTPREQUEST_H_UID000003466406882C
#define _HTTPREQUEST_H_UID000003466406882C

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

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
		HTTP_METHOD		eMethod = HTTP_GET;
		std::string		sUrl;
		//std::string		sBody;

		std::list<std::pair<std::string, std::string>> headers;
		std::string		sUserAgent;
		std::string		sReferer;

		int				nHttpTimeoutSec = 5 * 60;
		bool			bKnownBadSslCertificate = false;
	};

	struct Reply
	{
		bool			bSucceeded = false;
		std::string		sErrorDescription;

		int				nHttpCode = 0;
		std::string		sEffectiveUrl;
		std::string		sContentType;

		std::string		sBody;
	};

public:
	Reply DoRequest(const Request& requestInfo, const bool bVerbose);
};

//------------------------------------------------------
#endif //ifndef _HTTPREQUEST_H_UID000003466406882C
