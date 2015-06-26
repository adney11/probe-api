//------------------------------------------------------
#ifndef _COMMON_OPTIONS_H_UID00000HSBWKNSWIAJ8271
#define _COMMON_OPTIONS_H_UID00000HSBWKNSWIAJ8271

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//------------------------------------------------------

class OptionBase
{
public:
	virtual ~OptionBase()
	{
	};
	virtual bool ProcessCommandLineArg(const std::string& sArg, const bool bLastArg, const std::string& sNextArg, bool& bNextArgUsed) = 0;

protected:
	bool CheckArgument(const std::string& str) const
	{
		auto iter = std::find(vectAllowedArguments.cbegin(), vectAllowedArguments.cend(), str);
		return iter != vectAllowedArguments.cend();
	}

protected:
	std::vector<std::string>	vectAllowedArguments;
};

//------------------------------------------------------
// Option class is used to encapsulate all knowledge about some command line option:
// default value, minimal allowed value, maximum allowed value,
// allowed command line arguments to set this option.

template<class T>
class Option : public OptionBase
{
public:
	// for initialization as member of another class:
	Option(const T val, const T nMin, const T nMax, const std::vector<std::string>& vect)
		: data(val), minVal(nMin), maxVal(nMax)
	{
		vectAllowedArguments = vect;
	}

	// for initialization as member of another class:
	Option(const T val, const std::vector<std::string>& vect)
		: data(val), minVal(std::numeric_limits<T>::min()), maxVal(std::numeric_limits<T>::max())
	{
		vectAllowedArguments = vect;
	}

	operator T() const
	{
		return data;
	}

	Option& operator= (const T val)
	{
		data = val;
		return *this;
	}

	bool ProcessCommandLineArg(const std::string& sArg, const bool bLastArg, const std::string& sNextArg, bool& bNextArgUsed) override
	{
		if (!bLastArg && CheckArgument(sArg))
		{
			Parse(sNextArg);
			bNextArgUsed = true;
			return true;
		}
		return false;
	}

protected:
	void Parse(const std::string& str)
	{
		std::istringstream iss(str);
		T x = defVal;
		iss >> x;
		if (iss.fail())
		{
			throw std::out_of_range("Parsing failed for value: " + str);
		}
		if (x < minVal || x > maxVal)
		{
			throw std::out_of_range("Value is out of allowed range: " + str + " should be between " + std::to_string(minVal) + " and " + std::to_string(maxVal));
		}
		data = x;
	}

protected:
	const T defVal = 0;
	T	data = defVal;
	T	minVal = defVal;
	T	maxVal = defVal;
};

//------------------------------------------------------
// Option<bool> class is used to encapsulate all knowledge about BOOLEAN command line options:
// default value, allowed command line arguments to change this option.
// It gets some default value and changes it to opposite state once specified command line argument was found.

template<>
class Option<bool> : public OptionBase
{
public:
	typedef bool T;

	// for initialization as member of another class:
	Option(const T val, const std::vector<std::string>& vect)
		: defVal(val), data(val)
	{
		vectAllowedArguments = vect;
	}

	operator T() const
	{
		return data;
	}

	Option& operator= (const T val)
	{
		data = val;
		return *this;
	}

	bool ProcessCommandLineArg(const std::string& sArg, const bool bLastArg, const std::string& sNextArg, bool& bNextArgUsed) override
	{
		if (CheckArgument(sArg))
		{
			data = !defVal;
			return true;
		}
		return false;
	}

protected:
	T	defVal = false;
	T	data = false;
};

//------------------------------------------------------

struct CommonOptions
{
	const bool			bDebug;
	const bool			bVerbose;
	const std::string	sModeArgument;	// Country code in --list-asn mode
	const uint32_t		nCount;

	const std::string sMashapeUrl;
	const std::string sMashapeKey;

	CommonOptions(const bool bDebug_, const bool bVerbose_, const std::string& sModeArgument_, const uint32_t nCount_,
		const std::string& sMashapeUrl_, const std::string& sMashapeKey_) :
		bDebug(bDebug_), bVerbose(bVerbose_), sModeArgument(sModeArgument_), nCount(nCount_),
		sMashapeUrl(sMashapeUrl_), sMashapeKey(sMashapeKey_)
	{}
};

//------------------------------------------------------
#endif //ifndef _COMMON_OPTIONS_H_UID00000HSBWKNSWIAJ8271

