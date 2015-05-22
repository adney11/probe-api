//------------------------------------------------------
#include "stdafx.h"
#include "credits.h"

#include "Common.h"
#include "version.h"					// for VERSION_COPYRIGHT_2

#include <json/version.h>
#include <curlpp/cURLpp.hpp>

using namespace std;

//------------------------------------------------------

string GetPrintHelpSuggest()
{
	return OSSFMT("Use \"--help\" command line argument to get help!" << endl);
}

//------------------------------------------------------

string FormatRetCode(const int nRetCode)
{
	switch (nRetCode)
	{
#define DEF_RET_CODE(id)	case id: return #id
		DEF_RET_CODE(eRetCode::OK);
		DEF_RET_CODE(eRetCode::BadArguments);
		DEF_RET_CODE(eRetCode::NotSupported);
		DEF_RET_CODE(eRetCode::Cancelled);
		DEF_RET_CODE(eRetCode::ApiFailure);
		DEF_RET_CODE(eRetCode::ApiParsingFail);
		DEF_RET_CODE(eRetCode::OtherError);
		DEF_RET_CODE(eRetCode::HardFailure);
	}
	return to_string(nRetCode);
}

//------------------------------------------------------

string GetReturnCodeInfo()
{
	ostringstream buf;
#undef DEF_RET_CODE
#define DEF_RET_CODE(id)	buf << setw(5) << id << " - " <<  FormatRetCode(id)  << endl
	DEF_RET_CODE(eRetCode::OK);
	DEF_RET_CODE(eRetCode::BadArguments);
	DEF_RET_CODE(eRetCode::NotSupported);
	DEF_RET_CODE(eRetCode::Cancelled);
	DEF_RET_CODE(eRetCode::ApiFailure);
	DEF_RET_CODE(eRetCode::ApiParsingFail);
	DEF_RET_CODE(eRetCode::OtherError);
	DEF_RET_CODE(eRetCode::HardFailure);
	return buf.str();
}

//------------------------------------------------------

string GetCurlFullVersion()
{
	string res = curlpp::libcurlVersion();
	curl_version_info_data* pCurlInfo = curl_version_info(CURLVERSION_NOW);
	if (pCurlInfo)
	{
		res += "; Protocols:";
		for (const char* const* p = pCurlInfo->protocols; *p != nullptr; ++p)
		{
			res += " ";
			res += *p;
		}
	}
	return res;
}

//------------------------------------------------------

string FormatLibraryInfo(
	const string& sName,
	const string& sVersion,
	const string& sWebsite,
	const string& sRepository,
	const string& sLicenseType,
	const string& sLicenseURL)
{
	ostringstream buf;
	const size_t nTab1 = 4;
	const size_t nTab2 = 12;
	const string sPad(nTab1, ' ');

	buf
		<< left
		<< setw(nTab2) << sName << ": " << sVersion << endl
		<< sPad << setw(nTab2 - nTab1) << "Website" << ": " << sWebsite << endl
		<< sPad << setw(nTab2 - nTab1) << "Repo" << ": " << sRepository << endl
		<< sPad << setw(nTab2 - nTab1) << "License" << ": " << sLicenseType << "; " << sLicenseURL << endl
		<< right
		;
	return buf.str();
}

//------------------------------------------------------

string GetPrintCredits()
{
	ostringstream buf;

	buf << VERSION_COPYRIGHT_2 << endl;

	buf << endl;

	buf << "Sources repository:  " << "https://github.com/optimal-software/probe-api" << endl
		<< "Latest license:      " << "https://github.com/optimal-software/probe-api/blob/master/LICENSE" << endl
		;

	buf << endl;

	buf << "Authors: "
		<< implode(vector < string > {"Sergey Kolomenkin"}, ", ")
		<< endl;

	buf << R"(
===============================================================================

The MIT License (MIT)

Copyright (c) 2015 ProbeAPI Tools

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

===============================================================================
)";

	buf << endl;

	buf << "Used third-party libraries:" << endl;
	buf << FormatLibraryInfo("cURL", GetCurlFullVersion(), "http://curl.haxx.se/", "https://github.com/bagder/curl",
		"MIT", "http://curl.haxx.se/docs/copyright.html");
	buf << FormatLibraryInfo("cURLpp", LIBCURLPP_VERSION, "http://rrette.com/curlpp.html", "https://github.com/jpbarrette/curlpp",
		"MIT", "http://www.curlpp.org/#license");
	buf << FormatLibraryInfo("JsonCpp", JSONCPP_VERSION_STRING, "https://github.com/open-source-parsers/jsoncpp", "https://github.com/open-source-parsers/jsoncpp",
		"Public Domain, MIT", "https://github.com/open-source-parsers/jsoncpp/blob/master/LICENSE");
#ifndef DEST_OS_WINDOWS
	buf << FormatLibraryInfo("OpenSSL", "????", "https://www.openssl.org/", "https://github.com/openssl/openssl",
		"BSD-based", "http://www.openssl.org/source/license.html");
#endif

	return buf.str();
}

//------------------------------------------------------
