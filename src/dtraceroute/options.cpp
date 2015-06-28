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
    --country code [-n probes] [-w timeout] [-h max_hops] [-v] {target_name}
    --asn id [-n probes] [-w timeout] [-h max_hops] [-v] {target_name}

Options:
    {target_name}  Destination host IP or domain name.

    --help          Display this help.
    --version       Display detailed program version, copyright notices.
    --list-country  List available countries.
    --list-asn code List ASNs for specified 2 letter country code.
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
                    This option has an alias: --probes probes
    -w timeout      Timeout in milliseconds to wait for single ping.
    -h max_hops     Maximum number of hops to search for target (aka max TTL).
    -v              Verbose output.

Advanced options:
    -wa timeout     Timeout in milliseconds to wait for all probes.
    -d              Do not resolve addresses to hostnames.
    -hf max_hops    Maximum number of failed hops in a row to stop
                    (default: 3).
    -l size         Send buffer size.
    -f              Set Don't Fragment flag in packet (IPv4-only).
    -4              Force using IPv4.
    -6              Force using IPv6.
    --api-key key   Set web API key.
    --api-url url   Set web API URL.
    --no-delays     Disable delays during printing of results to console.
    --debug         Additional debug output.

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
void PrintOption(const string& name, const T& v)
{
	cout << "options: " << setw(15) << left << name << resetiosflags(ios_base::adjustfield) << " = " << v << endl;
}

//------------------------------------------------------

ApplicationOptions::ApplicationOptions()
{
	sMashapeUrl = MASHAPE_API_URL;
	sMashapeKey = MASHAPE_API_ID;
}

//------------------------------------------------------

void ApplicationOptions::Print()
{
	if (!bVerbose && !bDebug)
	{
		return;
	}

	const OptionCol options = GetAllOptions();

	for (auto it : options)
	{
		const OptionBase* pOption = it.first;
		const string& sOptionName = it.second;
		const string sOptionValue = pOption->GetValueAsString();

		PrintOption(sOptionName, sOptionValue);
	}

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
	const unsigned nMaxFailedHopsEffective = (std::min)(nMaxFailedHops + 0, nMaxHop - nStartHop + 1);
	const auto nAdditionalRowTime = nTriesPerHop * nWaitBetweenPingsMs + (bResolveIp2Name ? 500 : 0);

	nTimeoutTotalMs =
		nMaxFailedHopsEffective * (nTimeoutPingMs * nTriesPerHop + nAdditionalRowTime) // max time for continuous sequence of rows full of timeouts
		+ 10 * (nTimeoutPingMs * (nTriesPerHop - 1) + 500 + nAdditionalRowTime)	// approx bad time for other rows
		+ 2000;	// time for server to send all jobs and gather all results
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

		const OptionCol options = GetAllOptions();

		for (int i = 1; i < argc; ++i)
		{
			const string sArg = argv[i];
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

			bool bKnownArgument = false;

			// Try parse simple option declared by members derived from OptionBase class:
			{
				const string sNextArg = i + 1 < argc ? argv[i + 1] : "";
				bool bNextArgUsed = false;

				for (auto it : options)
				{
					OptionBase* pOption = it.first;
					const bool bProcessed = pOption->ProcessCommandLineArg(sArg, bLastArg, sNextArg, bNextArgUsed);

					if (bProcessed && pOption->InfluencesOnTotalTimeot())
					{
						RecalculateTotalTimeout();
					}

					bKnownArgument = bKnownArgument || bProcessed;
				}

				if (bNextArgUsed)
				{
					++i;
				}
			}

			if (bKnownArgument)
			{
				// do nothing - we already parsed it
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
			// Target is parsed for BY_COUNTRY and BY_ASN modes only if
			// target was not set before AND argument does not start from "-":
			else if ((
#ifdef DO_BY_COUNTRY_BY_DEFAULT
				MODE_UNKNOWN == mode ||
#endif
				MODE_DO_BY_COUNTRY == mode || MODE_DO_BY_ASN == mode)
				&& !bTargetSet && !begins(sArg, "-"))
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

		if (bVerbose)
		{
			cout << "Verbose mode ON" << endl;
		}

		if (bDebug)
		{
			cout << "Debug mode ON" << endl;
		}

		// Check arguments consistency:
		if (nStartHop > nMaxHop)
		{
			throw PException("Maximum hop number can't be less than starting hop number!", eRetCode::BadArguments);
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
