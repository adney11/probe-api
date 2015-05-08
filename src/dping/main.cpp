//------------------------------------------------------

#include "stdafx.h"

#include "options.h"
#include "dping.h"

//------------------------------------------------------

#ifdef _MSC_VER
#pragma comment (lib, "common.lib")
#endif // _MSC_VER > 1000

//------------------------------------------------------

using namespace std;

//------------------------------------------------------

int main(int argc, char* argv[])
{
	ProgramOptions options;

	const int nCmdLineRes = options.ProcessCommandLine(argc, argv);
	if (nCmdLineRes != 0)
	{
		return nCmdLineRes;
	}

	const int nProgramRes = Dping(options);

	return nProgramRes;
}

//------------------------------------------------------
