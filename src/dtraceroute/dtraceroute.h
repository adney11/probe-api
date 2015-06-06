//------------------------------------------------------
#ifndef _DTRACEROUTE_H_UID0000092WH823AKEN
#define _DTRACEROUTE_H_UID0000092WH823AKEN

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "options.h"

//------------------------------------------------------

struct ApplicationStats;
extern ApplicationStats* g_pStats;

//------------------------------------------------------

struct ApplicationStats
{
	int64_t		nSent = 0;
	int64_t		nReceived = 0;
	clock_t		nStartTime = 0;

	std::string	sTarget;

	ApplicationStats(const std::string& sTarget_) : sTarget(sTarget_)
	{
		nStartTime = clock();
		g_pStats = this;
	}
	~ApplicationStats()
	{
		g_pStats = nullptr;
	}

	int64_t GetTimeElapsedMs() const
	{
		return int64_t(double(clock() - nStartTime) * 1000 / CLOCKS_PER_SEC);
	}
};

//------------------------------------------------------

int Application(const ApplicationOptions& options);

//------------------------------------------------------
#endif //ifndef _DTRACEROUTE_H_UID0000092WH823AKEN
