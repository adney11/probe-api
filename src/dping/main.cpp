//------------------------------------------------------

#include "stdafx.h"

#include "options.h"
#include "dping.h"

#include "common/Common.h"

#include <signal.h>

//------------------------------------------------------

#ifdef _MSC_VER
#pragma comment (lib, "common.lib")
#endif // _MSC_VER > 1000

//------------------------------------------------------

using namespace std;

//------------------------------------------------------

void signal_handler(int s)
{
	cout << "Caught signal " << s << ". Program terminated." << endl;

	switch (s)
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

	const int nProgramRes = Dping(options);

	return nProgramRes;
}

//------------------------------------------------------
