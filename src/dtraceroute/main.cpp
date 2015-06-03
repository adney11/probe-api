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

volatile bool g_bSignalCatched = false;
volatile int g_nSignalRetCode = eRetCode::OK;
unique_ptr<string>  g_psSignalMessage = nullptr;

//------------------------------------------------------

string decode_signal(const int signal)
{
	const char* name = nullptr;
	switch (signal)
	{
	case SIGINT:
		name = "SIGINT";
		break;
	case SIGILL:
		name = "SIGILL";
		break;
#ifdef WIN32
	case SIGABRT_COMPAT:
		name = "SIGABRT_COMPAT";
		break;
#endif
	case SIGFPE:
		name = "SIGFPE";
		break;
	case SIGSEGV:
		name = "SIGSEGV";
		break;
	case SIGTERM:
		name = "SIGTERM";
		break;
#ifdef WIN32
	case SIGBREAK:
		name = "SIGBREAK";
		break;
#endif
	case SIGABRT:
		name = "SIGABRT";
		break;
	default:
		return to_string(signal);
	}
	return string(name) + "(" + to_string(signal) + ")";
}

//------------------------------------------------------

void signal_handler(const int signal)
{
	if (g_bSignalCatched)
	{
		cerr << endl << endl << "DUPLICATED SIGNAL " << decode_signal(signal) << endl << flush;
		return;
	}

	ostringstream buf;
	buf << endl;
	buf << "Caught signal " << decode_signal(signal) << ". Program terminated." << endl;
	buf << endl;

	switch (signal)
	{
	case SIGINT:
#ifdef WIN32
	case SIGABRT_COMPAT:
	case SIGBREAK:
#endif
	case SIGTERM:
		g_nSignalRetCode = eRetCode::Cancelled;
	default:
		g_nSignalRetCode = eRetCode::HardFailure;
	}
#ifdef _MSC_VER
	g_psSignalMessage = make_unique<string>(buf.str());
#else
	g_psSignalMessage = unique_ptr<string>(new string(buf.str()));
#endif
	g_bSignalCatched = true;

	g_bTerminateProgram = true;
	HttpRequester::bTerminateAllRequests = true;

	MySleep(5000);
	cerr << endl << endl << "ABNORMAL PROGRAM ABANDON!" << endl
		<< buf.str() << flush;
	cout << flush;
	exit(g_nSignalRetCode);
}

//------------------------------------------------------

int main(int argc, char* argv[])
{
	int nProgramRes = eRetCode::OtherError;
	bool bVerbose = false;

	try
	{
		signal(SIGINT, signal_handler);
		signal(SIGILL, signal_handler);
#ifdef WIN32
		signal(SIGABRT_COMPAT, signal_handler);
		signal(SIGBREAK, signal_handler);
#endif
		signal(SIGFPE, signal_handler);
		signal(SIGSEGV, signal_handler);
		signal(SIGTERM, signal_handler);
		signal(SIGABRT, signal_handler);

		ApplicationOptions options;

		const int nCmdLineRes = options.ProcessCommandLine(argc, argv);
		if (nCmdLineRes != eRetCode::NoValue)
		{
			return nCmdLineRes;
		}

		bVerbose = options.bVerbose || options.bDebug;
		options.Print();

		nProgramRes = Application(options);
	}
	catch (PException& e)
	{
		if (!g_bSignalCatched || bVerbose)
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
		cerr << *g_psSignalMessage;
		g_psSignalMessage = nullptr;
		nProgramRes = g_nSignalRetCode;
	}

	cerr << flush;
	cout << flush;
	return nProgramRes;
}

//------------------------------------------------------
