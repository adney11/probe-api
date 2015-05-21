//------------------------------------------------------

#include "stdafx.h"

#include "common/Common.h"

#include <signal.h>

//------------------------------------------------------

#ifdef _MSC_VER
#pragma comment (lib, "common.lib")
#endif // _MSC_VER > 1000

//------------------------------------------------------

using namespace std;

//------------------------------------------------------

bool g_bSignalCatched = false;
int g_nSignalRetCode = eRetCode::OK;
unique_ptr<string>  g_psSignalMessage = nullptr;

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
#if 1
	if(g_bSignalCatched)
		return;

	ostringstream buf;
	buf << endl;
	buf << "Caught signal " << decode_signal(signal) << ". Program terminated." << endl;
	buf << endl;

	switch (signal)
	{
	case SIGINT:
	case SIGABRT_COMPAT:
	case SIGTERM:
	case SIGBREAK:
		g_nSignalRetCode = eRetCode::Cancelled;
	default:
		g_nSignalRetCode = eRetCode::HardFailure;
	}
	g_psSignalMessage = make_unique<string>(buf.str());
	g_bSignalCatched = true;

	g_bTerminateProgram = true;
	HttpRequester::bTerminateAllRequests = true;

	MySleep(5000);
	cout << flush  << endl << endl << "ABNORMAL PROGRAM ABANDON!" << endl << flush;
	if (g_pApplicationStats)
	{
		g_pApplicationStats->Print();
	}
	cout << *g_psSignalMessage << flush;
	exit(g_nSignalRetCode);
#else
	HttpRequester::bPauseAllRequests = true;
	MySleep(100);

	cerr << flush;
	cout << flush;

	if (g_pApplicationStats)
	{
		g_pApplicationStats->Print();
	}

	ostringstream buf;
	buf << endl;
	buf << endl;
	buf << "Caught signal " << decode_signal(signal) << ". Program terminated." << endl;
	buf << endl;

	cout << buf.str();
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
#endif
}

//------------------------------------------------------

int main(int argc, char* argv[])
{
	int nProgramRes = eRetCode::OtherError;

	try
	{
		signal(SIGINT, signal_handler);
		signal(SIGILL, signal_handler);
		signal(SIGABRT_COMPAT, signal_handler);
		signal(SIGFPE, signal_handler);
		signal(SIGSEGV, signal_handler);
		signal(SIGTERM, signal_handler);
		signal(SIGBREAK, signal_handler);
		signal(SIGABRT, signal_handler);

		ApplicationOptions options;

		const int nCmdLineRes = options.ProcessCommandLine(argc, argv);
		if (nCmdLineRes != eRetCode::NoValue)
		{
			return nCmdLineRes;
		}

		options.Print();

		nProgramRes = Application(options);
	}
	catch (PException& e)
	{
		if (!g_bSignalCatched)
			cerr << "ERROR! " << e.what() << endl;
		nProgramRes = eRetCode::HardFailure;
	}
	catch (exception& e)
	{
		cerr << "ERROR! " << e.what() << endl;
		nProgramRes = eRetCode::HardFailure;
	}

	if (g_bSignalCatched)
	{
		cout << *g_psSignalMessage;
		nProgramRes = g_nSignalRetCode;
	}

	cerr << flush;
	cout << flush;

	return nProgramRes;
}

//------------------------------------------------------
