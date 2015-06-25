//------------------------------------------------------
#include "stdafx.h"
#include "options.h"

#include "common/Common.h"
#include "common/config.h"			// for MASHAPE_API_URL, MASHAPE_API_ID
#include "common/credits.h"
#include "common/version.h"			// for VERSION_PRODUCT_NAME, FILE_INTERNAL_NAME, MAIN_PRODUCT_VERSION_STR_A

using namespace std;

//------------------------------------------------------

//#define DO_BY_COUNTRY_BY_DEFAULT

//------------------------------------------------------

string GetPrintVersion()
{
	return OSSFMT(VERSION_PRODUCT_NAME ": " FILE_INTERNAL_NAME " v." MAIN_PRODUCT_VERSION_STR_A << endl);
}

//------------------------------------------------------

string GetPrintHelp(const ApplicationOptions& options)
{
	const string sHelpInfo = R"(
Usage: )" FILE_INTERNAL_NAME R"( --help
    --version
    --list-country [-v] [--debug]
    --list-asn code [-v] [--debug]
    --country code [-n probes] [-w timeout] [-wa timeout] [--no-delays] [-v]
                   [--debug] {target_name}
    --asn id [-n probes] [-w timeout] [-wa timeout] [--no-delays] [-v]
             [--debug] {target_name}

Options:
    {target_name}  Destination host IP or domain name.

    --help          Display this help.
    --version       Display detailed program version, copyright notices.
    --country code  Specify source addresses 2 letter country code
                    (ISO 3166-1 alpha-2).)"
#ifdef DO_BY_COUNTRY_BY_DEFAULT
R"(
                    Using source addresses from country with most of probes
                    available is a default setting.)"
#endif
R"(
    --asn id        Use source addresses from specified ASN
                    (autonomous system number) network.
    -n probes       Probes limit: number of hosts to make requests from.
    -w timeout      Timeout in milliseconds to wait for single ping.
    -wa timeout     Timeout in milliseconds to wait for all probes.
    --list-country  List available countries.
    --list-asn code List ASNs for specified 2 letter country code.
    --no-delays     Disable delays during printing of results to console.
    -v              Verbose output
    --debug         Additional debug output
)"
+ (options.bDebug ? GetDebugArgumentsHelp() : "")
+ "\n"
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
	cout << "options: " << setw(10) << left << name << resetiosflags(ios_base::adjustfield) << " = " << v << endl;
}

//------------------------------------------------------

ApplicationOptions::ApplicationOptions()
{
	sMashapeUrl = MASHAPE_API_URL;
	sMashapeKey = MASHAPE_API_ID;
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
	PrintOption("noDelays", bNoDelays);
	PrintOption("ping timeout", nTimeoutPingMs);
	PrintOption("total timeout", nTimeoutTotalMs);
	PrintOption("WaitBetweenPings", nWaitBetweenPingsMs);
	PrintOption("PingsPerProbe", nPingsPerProbe);
	PrintOption("nProbesLimit", nProbesLimit);
	PrintOption("nResultsLimit", nResultsLimit);
	//PrintOption("packet size", nPacketSize);
	PrintOption("ttl", nTTL);
	PrintOption("mode", mode);
	if (!sModeArgument.empty())
		PrintOption("mode arg", sModeArgument);
	if (!sTarget.empty())
		PrintOption("target", sTarget);
	PrintOption("MashapeUrl", sMashapeUrl);
	PrintOption("MashapeKey", sMashapeKey);
}

//------------------------------------------------------

void ApplicationOptions::RecalculateTotalTimeout()
{
	nTimeoutTotalMs = nTimeoutPingMs + 2000;
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
		RecalculateTotalTimeout();

		for (int i = 1; i < argc; ++i)
		{
			const string sArg = argv[i];
			//const bool bFirstArg = (1 == i);
			const bool bLastArg = (i + 1 == argc);

			if (g_bTerminateProgram)
				throw PException("ProcessCommandLine: Terminate Program");

			if (sArg == "--help")
			{
				cout << GetPrintVersion();
				cout << GetPrintHelp(*this);
				return eRetCode::OK;
			}
#ifdef DEST_OS_WINDOWS
			if (sArg == "/?")
			{
				cout << GetPrintVersion();
				cout << GetPrintHelp(*this);
				return eRetCode::OK;
			}
#endif
			if (sArg == "--version")
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
			else if (sArg == "--no-delay" || sArg == "--no-delays" || sArg == "--no-wait")
			{
				bNoDelays = true;
			}
			else if ((sArg == "-n" || sArg == "--probes") && !bLastArg)
			{
				const string sNextArg = argv[++i];
				CheckArgumentParameterNotEmpty(sArg, sNextArg);
				nProbesLimit = stoui32(sNextArg);
				nResultsLimit = stoui32(sNextArg);	// for list ASNs and list countries modes ONLY
			}
			else if (sArg == "-w" && !bLastArg)
			{
				const string sNextArg = argv[++i];
				CheckArgumentParameterNotEmpty(sArg, sNextArg);
				nTimeoutPingMs = stoui32(sNextArg);
				RecalculateTotalTimeout();
			}
			else if (sArg == "-wt" && !bLastArg)
			{
				const string sNextArg = argv[++i];
				CheckArgumentParameterNotEmpty(sArg, sNextArg);
				nTimeoutTotalMs = stoui32(sNextArg);
			}
			else if (sArg == "--api-url" && !bLastArg)
			{
				const string sNextArg = argv[++i];
				CheckArgumentParameterNotEmpty(sArg, sNextArg);
				sMashapeUrl = sNextArg;
			}
			else if (sArg == "--api-key" && !bLastArg)
			{
				const string sNextArg = argv[++i];
				CheckArgumentParameterNotEmpty(sArg, sNextArg);
				sMashapeKey = sNextArg;
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
			}
			else if (sArg == "--list-country" || sArg == "--list-countries")
			{
				mode = MODE_GET_COUNTRIES;
				sModeArgument.clear();
			}
			else if ((sArg == "--list-asn" || sArg == "--list-asns") && !bLastArg)
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
