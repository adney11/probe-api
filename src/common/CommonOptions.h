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

	virtual std::string GetValueAsString() const = 0;	// for verbose printing on screen

	bool InfluencesOnTotalTimeot() const
	{
		return m_bChangesTotalTimeot;
	}

protected:
	bool CheckArgument(const std::string& str) const
	{
		auto iter = std::find(m_vectArgumentNames.cbegin(), m_vectArgumentNames.cend(), str);
		return iter != m_vectArgumentNames.cend();
	}

protected:
	bool						m_bChangesTotalTimeot = false;
	std::vector<std::string>	m_vectArgumentNames;
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
	Option(const T val, const T nMin, const T nMax, const std::vector<std::string>& vect, const bool bChangesTotalTimeot = false)
		: m_data(val), m_minVal(nMin), m_maxVal(nMax)
	{
		m_vectArgumentNames = vect;
		m_bChangesTotalTimeot = bChangesTotalTimeot;
	}

	// for initialization as member of another class:
	Option(const T val, const std::vector<std::string>& vect, const bool bChangesTotalTimeot = false)
		: m_data(val), m_minVal(std::numeric_limits<T>::min()), m_maxVal(std::numeric_limits<T>::max())
	{
		m_vectArgumentNames = vect;
		m_bChangesTotalTimeot = bChangesTotalTimeot;
	}

	operator T() const
	{
		return m_data;
	}

	T GetValue() const
	{
		return m_data;
	}

	Option& operator= (const T val)
	{
		m_data = val;
		return *this;
	}

	virtual bool ProcessCommandLineArg(const std::string& sArg, const bool bLastArg, const std::string& sNextArg, bool& bNextArgUsed) override
	{
		if (!bLastArg && CheckArgument(sArg))
		{
			Parse(sNextArg, sArg);
			bNextArgUsed = true;
			return true;
		}
		return false;
	}

	virtual std::string GetValueAsString() const override	// for verbose printing on screen
	{
		return std::to_string(m_data);
	}

protected:
	void Parse(const std::string& str, const std::string& sArgName)
	{
		std::istringstream iss(str);
		auto x = T(0) + 0;	// hack for sizeof(T)==1 types: increase size of type to int so it is parsed as a number by istringstream
		iss >> x;
		if (iss.fail())
		{
			throw std::out_of_range("Argument \"" + sArgName + "\": Parsing failed for value: " + str
				+ ". Value should be between " + std::to_string(m_minVal) + " and " + std::to_string(m_maxVal));
		}
		if (x < m_minVal || x > m_maxVal)
		{
			throw std::out_of_range("Value for \"" + sArgName + "\" argument is out of allowed range: " + str
				+ ". Value should be between " + std::to_string(m_minVal) + " and " + std::to_string(m_maxVal));
		}
		m_data = x;
	}

protected:
	T	m_data = 0;
	T	m_minVal = 0;
	T	m_maxVal = 0;
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
	Option(const T val, const std::vector<std::string>& vect, const bool bChangesTotalTimeot = false)
		: m_defVal(val), m_data(val)
	{
		m_vectArgumentNames = vect;
		m_bChangesTotalTimeot = bChangesTotalTimeot;
	}

	operator T() const
	{
		return m_data;
	}

	Option& operator= (const T val)
	{
		m_data = val;
		return *this;
	}

	virtual bool ProcessCommandLineArg(const std::string& sArg, const bool bLastArg, const std::string& sNextArg, bool& bNextArgUsed) override
	{
		if (CheckArgument(sArg))
		{
			m_data = !m_defVal;
			return true;
		}
		return false;
	}

	virtual std::string GetValueAsString() const override	// for verbose printing on screen
	{
		return m_data ? "ON" : "OFF";
	}

protected:
	T	m_defVal = false;
	T	m_data = false;
};

//------------------------------------------------------
// Allow correct printing to streams:

inline std::ostream& operator<<(std::ostream &os, const OptionBase& option)
{
	return os << option.GetValueAsString();
}

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

