//------------------------------------------------------
#include "stdafx.h"
#include "options.h"

#include "common/Common.h"
#include "common/version.h"			// for VERSION_PRODUCT_NAME, FILE_INTERNAL_NAME, MAIN_PRODUCT_VERSION_STR_A

#include <json/version.h>
#include <curlpp/cURLpp.hpp>

using namespace std;

//#define ALLOW_PINGING_BY_DEFAULT

//------------------------------------------------------

ProgramOptions::ProgramOptions()
	: bVerbose(false)
	, bDebug(false)
	, nMaxTimeoutMs(DEFAULT_PING_TIMEOUT)
	, nPingCount(DEFAULT_PING_COUNT)
	, mode(MODE_UNKNOWN)
	, nTTL(DEFAULT_PING_TTL)
	, nPacketSize(DEFAULT_PING_PACKET_SIZE)
{

}

//------------------------------------------------------

string GetPrintVersion()
{
	return OSSFMT(VERSION_PRODUCT_NAME ": " FILE_INTERNAL_NAME " v." MAIN_PRODUCT_VERSION_STR_A << endl);
}

//------------------------------------------------------

string GetPrintHelpSuggest()
{
	return OSSFMT("Use \"--help\" command line argument to get help!" << endl);
}

//------------------------------------------------------

string GetPrintHelp()
{
	const string sHelpInfo = R"zzz(
Usage: dping [--help]
             [--version]
             --list-country [-v] [--debug]
             --list-asn code [-v] [--debug]
             --country code [-n count] [-w timeout] [-v] [--debug] {target_name}
             --asn id [-n count] [-w timeout] [-v] [--debug] {target_name}

Options:
    {target_name}  Destination host IP or domain name.

    --help          Display this help.
    --version       Display detailed program version, copyright notices.
    --country code  Ping from specified 2 letter country code (ISO 3166-1 alpha-2).)zzz"
#ifdef ALLOW_PINGING_BY_DEFAULT
R"zzz(
                    Pinging by country with most probes available is a default setting.)zzz"
#endif
R"zzz(
    --asn id        Ping from specified ASN (autonomous system number) network.
    -n count        Number of echo requests to send.
    -w timeout      Timeout in milliseconds to wait for each reply.
    --list-country  List available countries.
    --list-asn code List ASNs for specified 2 letter country code.
    -v              Verbose output
    --debug         Additional debug output

Return Codes:
)zzz"
#define HELP_RET_CODE(id)	+ OSSFMT(setw(5) << id << " - " #id  << endl)
HELP_RET_CODE(eRetCode::OK)
HELP_RET_CODE(eRetCode::BadArguments)
HELP_RET_CODE(eRetCode::NotSupported)
HELP_RET_CODE(eRetCode::Cancelled)
HELP_RET_CODE(eRetCode::ApiFailure)
HELP_RET_CODE(eRetCode::OtherError)
HELP_RET_CODE(eRetCode::HardFailure)
+ R"zzz(
Examples:
dping --list-country
dping --list-asn ES
dping --country US 8.8.8.8
dping --asn AS3352 8.8.8.8

)zzz";

	return sHelpInfo;
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

	buf << GetPrintVersion();
	buf << VERSION_COPYRIGHT_2 << endl;

	buf << endl;

	buf << "Sources repository:  " << "https://github.com/optimal-software/probe-api" << endl
		<< "Latest license:      " << "https://github.com/optimal-software/probe-api/blob/master/LICENSE" << endl
		;

	buf << endl;

	buf << "Authors: "
		<< implode(vector < string > {"Sergey Kolomenkin"}, ", ")
		<< endl;

	buf << R"zzz(
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
)zzz";

	buf << endl;

	buf << "Used third-party libraries: " << endl;
	buf << FormatLibraryInfo("cURL", GetCurlFullVersion(), "http://curl.haxx.se/", "https://github.com/bagder/curl",
		"MIT", "http://curl.haxx.se/docs/copyright.html");
	buf << FormatLibraryInfo("cURLpp", LIBCURLPP_VERSION, "http://rrette.com/curlpp.html", "https://github.com/jpbarrette/curlpp",
		"MIT", "http://www.curlpp.org/#license");
	buf << FormatLibraryInfo("jsoncpp", JSONCPP_VERSION_STRING, "https://github.com/open-source-parsers/jsoncpp", "https://github.com/open-source-parsers/jsoncpp",
		"Public Domain, MIT", "https://github.com/open-source-parsers/jsoncpp/blob/master/LICENSE");
#ifndef OS_WINDOWS
	buf << FormatLibraryInfo("OpenSSL", "????", "https://www.openssl.org/", "https://github.com/openssl/openssl",
		"BSD-based", "http://www.openssl.org/source/license.html");
#endif

	return buf.str();
}

//------------------------------------------------------

void CheckArgumentParameterNotEmpty(const string& sArg, const string& sParam)
{
	if (sParam.empty())
	{
		throw exception(OSSFMT("Empty value is not allowed for command line argument \"" << sArg << "\".").c_str());
	}
}

//------------------------------------------------------

int ProgramOptions::ProcessCommandLine(const int argc, const char* const argv[])	// returns non-zero result if process exit required
{
	try
	{
		if (argc <= 1)
		{
			cerr << GetPrintVersion();
			throw exception("The syntax of the command is incorrect.");
		}

		bool bTargetSet = false;

		for (int i = 1; i < argc; ++i)
		{
			const string sArg = argv[i];
			const bool bFirstArg = (1 == i);
			const bool bLastArg = (i + 1 == argc);

			if (bFirstArg && sArg == "--help")
			{
				cout << GetPrintVersion();
				cout << GetPrintHelp();
				return eRetCode::OK;
			}
#ifdef OS_WINDOWS
			if (bFirstArg && sArg == "/?")
			{
				cout << GetPrintVersion();
				cout << GetPrintHelp();
				return eRetCode::OK;
			}
#endif
			if (bFirstArg && sArg == "--version")
			{
				cout << GetPrintCredits();
				return eRetCode::OK;
			}

			if (sArg == "-v")
			{
				bVerbose = true;
			}
			else if (sArg == "--debug")
			{
				bDebug = true;
			}
			else if (sArg == "-n" && !bLastArg)
			{
				const string sNextArg = argv[++i];
				CheckArgumentParameterNotEmpty(sArg, sNextArg);
				nPingCount = stoul(sNextArg);
			}
			else if (sArg == "-w" && !bLastArg)
			{
				const string sNextArg = argv[++i];
				CheckArgumentParameterNotEmpty(sArg, sNextArg);
				nMaxTimeoutMs = stoul(sNextArg);
			}
			else if (sArg == "--country" && !bLastArg)
			{
				const string sNextArg = argv[++i];
				CheckArgumentParameterNotEmpty(sArg, sNextArg);
				mode = MODE_PING_BY_COUNTRY;
				sModeArgument = sNextArg;
			}
			else if (sArg == "--asn" && !bLastArg)
			{
				const string sNextArg = argv[++i];
				CheckArgumentParameterNotEmpty(sArg, sNextArg);
				mode = MODE_PING_BY_ASN;
				sModeArgument = sNextArg;
			}
			else if (sArg == "--list-country")
			{
				mode = MODE_GET_COUNTRIES;
				sModeArgument.clear();
			}
			else if (sArg == "--list-asn" && !bLastArg)
			{
				const string sNextArg = argv[++i];
				CheckArgumentParameterNotEmpty(sArg, sNextArg);
				mode = MODE_GET_ASNS;
				sModeArgument = sNextArg;
			}
			else if ((
#ifdef ALLOW_PINGING_BY_DEFAULT
				MODE_UNKNOWN == mode ||
#endif
				MODE_PING_BY_COUNTRY == mode || MODE_PING_BY_ASN == mode) && bLastArg)
			{
				CheckArgumentParameterNotEmpty("{target}", sArg);
				sTarget = sArg;
				bTargetSet = true;
			}
			else
			{
				throw exception(OSSFMT("Unknown command line argument \"" << sArg << "\" or command line is missing required parameter.").c_str());
			}
		}

		if (MODE_UNKNOWN == mode)
		{
#ifdef ALLOW_PINGING_BY_DEFAULT
			mode = MODE_PING_BY_COUNTRY;
			sModeArgument = DEFAULT_PING_COUNTRY_META;
#else
			throw exception("Program mode is not specified.");
#endif
		}

		if ((MODE_PING_BY_COUNTRY == mode || MODE_PING_BY_ASN == mode) && !bTargetSet)
		{
			throw exception("Ping target is not specified!");
		}
	}
	catch (exception& e)
	{
		cerr << "ERROR! " << e.what() << endl;
		cerr << GetPrintHelpSuggest();
		return eRetCode::BadArguments;
	}

	//bVerbose = true;

	return eRetCode::NoValue;
}

//------------------------------------------------------
