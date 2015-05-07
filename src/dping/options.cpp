//------------------------------------------------------
#include "stdafx.h"
#include "options.h"

#include "common\version.h"			// for VERSION_PRODUCT_NAME, FILE_INTERNAL_NAME, MAIN_PRODUCT_VERSION_STR_A

#include <iostream>

using namespace std;

//------------------------------------------------------

ProgramOptions::ProgramOptions() : bVerbose(false)
{

}

//------------------------------------------------------

int ProgramOptions::ProcessCommandLine(const int argc, const char* const argv[])	// returns non-zero result if process exit required
{
	cout << VERSION_PRODUCT_NAME ": " FILE_INTERNAL_NAME " v." MAIN_PRODUCT_VERSION_STR_A << endl;

	for (int i = 1; i < argc; ++i)
	{
		const string sArg = argv[i];
		if (1 == i && sArg == "--help")
		{
			cout << FILE_INTERNAL_NAME " usage:" << endl;
			cout << "\t" FILE_INTERNAL_NAME " --help\t\t- to get this help" << endl;
			cout << "\t" FILE_INTERNAL_NAME " -v\t\t- to get program version" << endl;
			return 100;
		}
		if (1 == i && sArg == "-v")
		{
			return 100;
		}
	}

	//bVerbose = true;

	return 0;
}

//------------------------------------------------------
