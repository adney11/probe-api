//------------------------------------------------------
#include "stdafx.h"
#include "options.h"

#include "common/Common.h"
#include "common/credits.h"
#include "common/version.h"			// for VERSION_PRODUCT_NAME, FILE_INTERNAL_NAME, MAIN_PRODUCT_VERSION_STR_A

using namespace std;

//------------------------------------------------------

//#define DO_BY_COUNTRY_BY_DEFAULT

//------------------------------------------------------

ApplicationOptions::ApplicationOptions()
	: bVerbose(false)
	, bDebug(false)
	, nMaxTimeoutMs(DEFAULT_TRACERT_TIMEOUT)
	, nCount(DEFAULT_TRACERT_COUNT)
	, nTTL(DEFAULT_TRACERT_TTL)
	, nPacketSize(DEFAULT_TRACERT_PACKET_SIZE)
	, mode(MODE_UNKNOWN)
{
}

//------------------------------------------------------

string GetPrintVersion()
{
	return OSSFMT(VERSION_PRODUCT_NAME ": " FILE_INTERNAL_NAME " v." MAIN_PRODUCT_VERSION_STR_A << endl);
}

//------------------------------------------------------

string GetPrintHelp()
{
	const string sHelpInfo = R"(
Usage: )" FILE_INTERNAL_NAME R"( --help
    --version
    --list-country [-v] [--debug]
    --list-asn code [-v] [--debug]
    --country code [-n count] [-w timeout] [-h maximum_hops] [-v] [--debug] {target_name}
    --asn id [-n count] [-w timeout] [-h maximum_hops] [-v] [--debug] {target_name}

Options:
    {target_name}  Destination host IP or domain name.

    --help          Display this help.
    --version       Display detailed program version, copyright notices.
    --country code  Specify source addresses 2 letter country code (ISO 3166-1 alpha-2).)"
#ifdef DO_BY_COUNTRY_BY_DEFAULT
R"(
                    Using source addresses from country with most of hosts available is a default setting.)"
#endif
R"(
    --asn id        Use source addresses from specified ASN (autonomous system number) network.
    -n count        Number of echo requests to send.
    -w timeout      Timeout in milliseconds to wait for each reply.
    -h maximum_hops Maximum number of hops to search for target.
    --list-country  List available countries.
    --list-asn code List ASNs for specified 2 letter country code.
    -v              Verbose output
    --debug         Additional debug output

Return Codes:
)"
+ GetReturnCodeInfo()
+ R"(
Examples:
)" FILE_INTERNAL_NAME R"( --list-country
)" FILE_INTERNAL_NAME R"( --list-asn ES
)" FILE_INTERNAL_NAME R"( --country US 8.8.8.8
)" FILE_INTERNAL_NAME R"( --asn AS3352 8.8.8.8

)";

	return sHelpInfo;
}

//------------------------------------------------------

void CheckArgumentParameterNotEmpty(const string& sArg, const string& sParam)
{
	if (sParam.empty())
	{
		throw PException(eRetCode::BadArguments) << "Empty value is not allowed for command line argument \"" << sArg << "\".";
	}
}

//------------------------------------------------------

template<class T>
void PrintOption(const char* name, const T& v)
{
	cout << "options: " << setw(10) << left << name << right << " = " << v << endl;
}

//------------------------------------------------------

void ApplicationOptions::Print() const
{
	if (!bVerbose && !bDebug)
	{
		return;
	}

	PrintOption("verbose", bVerbose);
	PrintOption("debug", bDebug);
	PrintOption("timeout", nMaxTimeoutMs);
	PrintOption("count", nCount);
	//PrintOption("packet size", nPacketSize);
	PrintOption("ttl", nTTL);
	PrintOption("mode", mode);
	if (!sModeArgument.empty())
		PrintOption("mode arg", sModeArgument);
	if (!sTarget.empty())
		PrintOption("target", sTarget);
}

//------------------------------------------------------

int ApplicationOptions::ProcessCommandLine(const int argc, const char* const argv[])	// returns non-zero result if process exit required
{
	try
	{
		if (argc <= 1)
		{
			cerr << GetPrintVersion();
			throw PException("The syntax of the command is incorrect.", eRetCode::BadArguments);
		}

		bool bTargetSet = false;

		for (int i = 1; i < argc; ++i)
		{
			const string sArg = argv[i];
			const bool bFirstArg = (1 == i);
			const bool bLastArg = (i + 1 == argc);

			if (g_bTerminateProgram)
				throw PException("ProcessCommandLine: Terminate Program");

			if (bFirstArg && sArg == "--help")
			{
				cout << GetPrintVersion();
				cout << GetPrintHelp();
				return eRetCode::OK;
			}
#ifdef DEST_OS_WINDOWS
			if (bFirstArg && sArg == "/?")
			{
				cout << GetPrintVersion();
				cout << GetPrintHelp();
				return eRetCode::OK;
			}
#endif
			if (bFirstArg && sArg == "--version")
			{
				cout << GetPrintVersion();
				cout << GetPrintCredits();
				return eRetCode::OK;
			}

			if (sArg == "-v")
			{
				bVerbose = true;
				cout << "Verbose mode ON" << endl;
			}
			else if (sArg == "--debug")
			{
				bDebug = true;
				cout << "Debug mode ON" << endl;
			}
			else if (sArg == "-n" && !bLastArg)
			{
				const string sNextArg = argv[++i];
				CheckArgumentParameterNotEmpty(sArg, sNextArg);
				nCount = stoul(sNextArg);
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
				mode = MODE_DO_BY_COUNTRY;
				sModeArgument = sNextArg;
			}
			else if (sArg == "--asn" && !bLastArg)
			{
				const string sNextArg = argv[++i];
				CheckArgumentParameterNotEmpty(sArg, sNextArg);
				mode = MODE_DO_BY_ASN;
				sModeArgument = sNextArg;
				nCount = UINT_MAX;
			}
			else if (sArg == "--list-country")
			{
				mode = MODE_GET_COUNTRIES;
				sModeArgument.clear();
				nCount = UINT_MAX;
			}
			else if (sArg == "--list-asn" && !bLastArg)
			{
				const string sNextArg = argv[++i];
				CheckArgumentParameterNotEmpty(sArg, sNextArg);
				mode = MODE_GET_ASNS;
				sModeArgument = sNextArg;
			}
			else if ((
#ifdef DO_BY_COUNTRY_BY_DEFAULT
				MODE_UNKNOWN == mode ||
#endif
				MODE_DO_BY_COUNTRY == mode || MODE_DO_BY_ASN == mode) && bLastArg)
			{
				CheckArgumentParameterNotEmpty("{target}", sArg);
				sTarget = sArg;
				bTargetSet = true;
			}
			else
			{
				throw PException(eRetCode::BadArguments) << "Unknown command line argument \"" << sArg << "\" or command line is missing required parameter.";
			}
		}

		if (MODE_UNKNOWN == mode)
		{
#ifdef DO_BY_COUNTRY_BY_DEFAULT
			mode = MODE_DO_BY_COUNTRY;
			sModeArgument = DEFAULT_COUNTRY_META;
#else
			throw PException("Program mode is not specified.", eRetCode::BadArguments);
#endif
		}

		if ((MODE_DO_BY_COUNTRY == mode || MODE_DO_BY_ASN == mode) && !bTargetSet)
		{
			throw PException("Target host is not specified!", eRetCode::BadArguments);
		}
	}
	catch (PException& e)
	{
		cerr << "ERROR! " << e.what() << endl;
		cerr << GetPrintHelpSuggest();
		return eRetCode::BadArguments;
	}

	//bVerbose = true;

	return eRetCode::NoValue;
}

//------------------------------------------------------