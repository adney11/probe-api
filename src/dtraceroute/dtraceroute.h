//------------------------------------------------------
#ifndef _DTRACEROUTE_H_UID0000092WH823AKEN
#define _DTRACEROUTE_H_UID0000092WH823AKEN

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "options.h"

//------------------------------------------------------

struct ApplicationStats
{
	int64_t		nSent = 0;
	int64_t		nReceived = 0;

	std::string	sTarget;

	ApplicationStats(const std::string& sTarget_) : sTarget(sTarget_)
	{}
};

//------------------------------------------------------

int Application(const ApplicationOptions& options);

//------------------------------------------------------
#endif //ifndef _DTRACEROUTE_H_UID0000092WH823AKEN
