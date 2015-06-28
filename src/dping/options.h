//------------------------------------------------------
#ifndef _OPTIONS_H_UID000003A649B0548E
#define _OPTIONS_H_UID000003A649B0548E

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "common/Common.h"			// for CommonOptions

//------------------------------------------------------

class ApplicationOptions
{
public:
	ApplicationOptions();
	void Print();
	void RecalculateTotalTimeout();
	int ProcessCommandLine(const int argc, const char* const argv[]);	// returns non-zero result if process exit required

	CommonOptions GetCommonOptions() const
	{
		return CommonOptions(bDebug, bVerbose, sModeArgument, nResultsLimit, sMashapeUrl, sMashapeKey);
	}

public:
	enum eMode
	{
		MODE_UNKNOWN,
		MODE_GET_COUNTRIES,
		MODE_GET_ASNS,
		MODE_DO_BY_COUNTRY,
		MODE_DO_BY_ASN,
	};

public:
	Option<bool>	bVerbose		= { false, { "-v" } };
	Option<bool>	bDebug			= { false, { "--debug" } };
	Option<bool>	bNoDelays		= { false, { "--no-delay", "--no-delays", "--no-wait" } };

	Option<bool>	bResolveIp2Name = { false, { "-a" },	true };	// this is false by default in Windows
	Option<bool>	bUseIpv4Only	= { false, { "-4" } };
	Option<bool>	bUseIpv6Only	= { false, { "-6" } };
	Option<bool>	bDontFragment	= { false, { "-f" } };
	Option<bool>	bInfinitePing	= { false, { "-t" } };

	// initialization sequence: { defaultValue, minValue, maxValue, {"--cmdArg"}, bChangesTimeoutTotalMs }
	Option<uint32_t>	nTimeoutPingMs		= { 3000,	100,	10000,		{ "-w" },	true };
	Option<uint32_t>	nTimeoutTotalMs		= { 0,		2000,	5*60*1000,	{ "-wt" } };
	Option<uint32_t>	nWaitBetweenPingsMs	= { 100,	1,		10000,		{ },		true };
	Option<uint32_t>	nPingsPerProbe		= { 1,		1,		10,			{ "-np" },	true };
	// following two options share the same command line argument; This is expected
	// since they have different limitations and they are used in different program run modes.
	const uint32_t nMinProbesToRequest = 10;
	Option<uint32_t>	nProbesLimit		= { 10,		10,		1000,		{ "-n", "--probes" } };
	Option<uint32_t>	nResultsLimit		= { std::numeric_limits<uint32_t>::max(), { "-n" } };		// for list ASNs and list countries modes; UINT32_MAX - display ALL items from requested list
	Option<uint16_t>	nPacketSize			= { 32,		0,		65500,		{ "-l" } };
	Option<uint8_t>		nTTL				= { 55,		1,		255,		{ "-i" } };

	eMode		mode			= MODE_UNKNOWN;
	std::string	sModeArgument;
	std::string	sTarget;

	std::string sMashapeUrl;
	std::string sMashapeKey;

protected:
	typedef std::vector<std::pair<OptionBase*, std::string>> OptionCol;
	OptionCol GetAllOptions()
	{
		OptionCol col;
		col.reserve(32);
		col.emplace_back(&bVerbose, "bVerbose");
		col.emplace_back(&bDebug, "bDebug");
		col.emplace_back(&bNoDelays, "bNoDelays");
		col.emplace_back(&bResolveIp2Name, "bResolveIp2Name");
		col.emplace_back(&bUseIpv4Only, "bUseIpv4Only");
		col.emplace_back(&bUseIpv6Only, "bUseIpv6Only");
		col.emplace_back(&bDontFragment, "bDontFragment");
		col.emplace_back(&bInfinitePing, "bInfinitePing");

		col.emplace_back(&nTimeoutPingMs, "nTimeoutPingMs");
		col.emplace_back(&nTimeoutTotalMs, "nTimeoutTotalMs");
		col.emplace_back(&nWaitBetweenPingsMs, "nWaitBetweenPingsMs");
		col.emplace_back(&nPingsPerProbe, "nPingsPerProbe");
		col.emplace_back(&nProbesLimit, "nProbesLimit");
		col.emplace_back(&nResultsLimit, "nResultsLimit");
		col.emplace_back(&nPacketSize, "nPacketSize");
		col.emplace_back(&nTTL, "nTTL");
		return col;
	}
};

//------------------------------------------------------
#endif //ifndef _OPTIONS_H_UID000003A649B0548E
