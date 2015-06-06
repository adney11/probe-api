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
	std::chrono::time_point<std::chrono::system_clock>	nStartTime;

	std::string	sTarget;

	ApplicationStats(const std::string& sTarget_) : sTarget(sTarget_)
	{
		nStartTime = std::chrono::system_clock::now();
		g_pStats = this;
	}
	~ApplicationStats()
	{
		g_pStats = nullptr;
	}

	int64_t GetTimeElapsedMs() const
	{
		return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - nStartTime).count();
	}
};

//------------------------------------------------------

int Application(const ApplicationOptions& options);

//------------------------------------------------------
#endif //ifndef _DTRACEROUTE_H_UID0000092WH823AKEN
