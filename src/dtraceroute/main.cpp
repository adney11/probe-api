//------------------------------------------------------

#include "stdafx.h"

#include "dtraceroute.h"

#include "common/Common.h"

#include <signal.h>

//------------------------------------------------------

#ifdef _MSC_VER
#pragma comment (lib, "common.lib")
#endif // _MSC_VER > 1000

//------------------------------------------------------

using namespace std;

//------------------------------------------------------

string decode_signal(const int signal)
{
	switch (signal)
	{
	case SIGINT:
		return "SIGINT";
	case SIGILL:
		return "SIGILL";
	case SIGABRT_COMPAT:
		return "SIGABRT_COMPAT";
	case SIGFPE:
		return "SIGFPE";
	case SIGSEGV:
		return "SIGSEGV";
	case SIGTERM:
		return "SIGTERM";
	case SIGBREAK:
		return "SIGBREAK";
	case SIGABRT:
		return "SIGABRT";
	default:
		return to_string(signal);
	}
}

//------------------------------------------------------

void signal_handler(const int signal)
{
	cout << flush;

	if (g_pPingStats)
	{
		g_pPingStats->Print();
	}

	cout << endl << "Caught signal " << decode_signal(signal) << ". Program terminated." << endl;

	cout << flush;

	switch (signal)
	{
	case SIGINT:
	case SIGABRT_COMPAT:
	case SIGTERM:
	case SIGBREAK:
		exit(eRetCode::Cancelled);
	default:
		exit(eRetCode::HardFailure);
	}
}

//------------------------------------------------------

int main(int argc, char* argv[])
{
	signal(SIGINT, signal_handler);
	signal(SIGILL, signal_handler);
	signal(SIGABRT_COMPAT, signal_handler);
	signal(SIGFPE, signal_handler);
	signal(SIGSEGV, signal_handler);
	signal(SIGTERM, signal_handler);
	signal(SIGBREAK, signal_handler);
	signal(SIGABRT, signal_handler);

	ProgramOptions options;

	const int nCmdLineRes = options.ProcessCommandLine(argc, argv);
	if (nCmdLineRes != eRetCode::NoValue)
	{
		return nCmdLineRes;
	}

	const int nProgramRes = Application(options);

	return nProgramRes;
}

//------------------------------------------------------
