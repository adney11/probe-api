//------------------------------------------------------
#ifndef _DPING_H_UID000003A64AF9C20F
#define _DPING_H_UID000003A64AF9C20F

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "options.h"

//------------------------------------------------------
// http://stackoverflow.com/questions/12140635/stdmultiset-and-finding-the-middle-element/30670542#30670542

template<class T>
double GetMedian(const std::multiset<T>& data)
{
	if (data.empty())
		return 0;

	const size_t n = data.size();
	double median = 0;

	auto iter = data.cbegin();
	std::advance(iter, n / 2);

	// Middle or average of two middle values
	if (n % 2 == 0)
	{
		const auto iter2 = iter--;
		median = double(*iter + *iter2) / 2;    // data[n/2 - 1] AND data[n/2]
	}
	else
	{
		median = *iter;
	}

	return median;
}

template<class T>
double GetMedianAbsoluteDeviation(const std::multiset<T>& data)
{
	const double fMedian = GetMedian(data);
	std::multiset<double> diff;
	for (const auto& x : data)
	{
		diff.insert(std::abs(fMedian - x));
	}
	return GetMedian(diff);
}

//------------------------------------------------------

struct MathCollection
{
	std::multiset<int>	data;
	int			nMin = std::numeric_limits<int>::max();
	int			nMax = 0;
	int64_t		nSum = 0;

	void AddItem(const int x)
	{
		data.insert(x);
		nMin = (std::min)(nMin, x);
		nMax = (std::max)(nMax, x);
		nSum += x;
	}

	int GetMin() const
	{
		return nMin;
	}

	int GetMax() const
	{
		return nMax;
	}

	double GetAverage() const
	{
		return double(nSum) / data.size();
	}

	double GetMedian() const
	{
		return ::GetMedian(data);
	}

	double GetMedianAbsoluteDeviation() const
	{
		return ::GetMedianAbsoluteDeviation(data);
	}
};

//------------------------------------------------------

struct ApplicationStats;
extern std::shared_ptr<ApplicationStats> g_ptrStats;

//------------------------------------------------------

struct ApplicationStats
{
	int64_t		nSent = 0;
	int64_t		nReceived = 0;
	std::chrono::time_point<std::chrono::system_clock>	nStartTime;
	MathCollection	pings;

	std::string	sTarget;

	ApplicationStats(const std::string& sTarget_) : sTarget(sTarget_)
	{
		nStartTime = std::chrono::system_clock::now();
	}
	~ApplicationStats()
	{
	}

	int64_t GetTimeElapsedMs() const
	{
		return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - nStartTime).count();
	}
};

//------------------------------------------------------

int Application(const ApplicationOptions& options);

//------------------------------------------------------
#endif //ifndef _DPING_H_UID000003A64AF9C20F
