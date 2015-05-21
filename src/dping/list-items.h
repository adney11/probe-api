//------------------------------------------------------
#ifndef _DPING_H_UID000003A64AF9C20F
#define _DPING_H_UID000003A64AF9C20F

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "options.h"

//------------------------------------------------------

struct ApplicationStats
{
	int64_t		nSent = 0;
	int64_t		nReceived = 0;
	int			nPingMin = INT_MAX;
	int			nPingMax = 0;
	int64_t		nPingSum = 0;

	std::string	sTarget;

	ApplicationStats(const std::string& sTarget);
	~ApplicationStats();
	void Print();
};

extern ApplicationStats*	g_pApplicationStats;

//------------------------------------------------------

int Application(const ApplicationOptions& options);

//------------------------------------------------------
#endif //ifndef _DPING_H_UID000003A64AF9C20F
