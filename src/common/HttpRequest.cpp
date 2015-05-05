//------------------------------------------------------
#include "stdafx.h"
#include "HttpRequest.h"

#include <iostream>
#include <list>
#include <string>

#include <cstdlib>
#include <cstdio>
#include <cstring>

#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/Exception.hpp>

//------------------------------------------------------

#if _MSC_VER > 1000
#pragma comment (lib, "libcurl.lib")
#pragma comment (lib, "curlpp.lib")
// libcurl DLL dependencies:
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Wldap32.lib")
#endif // _MSC_VER > 1000

//------------------------------------------------------

void MakeHttpRequest()
{
	char *url = "https://google.com/";

	//char buf[50];
	try
	{
		curlpp::Cleanup cleaner;
		curlpp::Easy request;

		std::list<std::string> headers;
		//headers.push_back("Content-Type: text/*");
		//sprintf(buf, "Content-Length: %d", size);
		//headers.push_back(buf);

		using namespace curlpp::Options;
		request.setOpt(new Verbose(true));
		request.setOpt(new HttpHeader(headers));
		request.setOpt(new Url(url));

		request.perform();
	}
	catch (curlpp::LogicError & e)
	{
		std::cout << e.what() << std::endl;
	}
	catch (curlpp::RuntimeError & e)
	{
		std::cout << e.what() << std::endl;
	}
}



//------------------------------------------------------
