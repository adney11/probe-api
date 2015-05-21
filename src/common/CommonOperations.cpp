//------------------------------------------------------
#include "stdafx.h"
#include "CommonOperations.h"

#include "Common.h"
#include "ParseReply.h"

#include <json/json.h>

#include <algorithm>

using namespace std;

//------------------------------------------------------

string GetDefaultSourceCountry(ProbeApiRequester& requester, const CommonOptions& options)
{
	ProbeApiRequester::Request request("GetCountries");

	const ProbeApiRequester::Reply reply = requester.DoRequest(request, options.bDebug);
	if (!reply.bSucceeded)
	{
		throw exception(("GetDefaultCountryToPing: " + reply.sErrorDescription).c_str());
		return string();
	}

	using namespace ProbeAPI;
	vector<CountryInfo> countries;

	try
	{
		countries = ParseCountries(reply.sBody);
	}
	catch (exception& e)
	{
		throw exception((string("GetDefaultCountryToPing: ") + e.what()).c_str());
		return string();
	}

	sort(countries.begin(), countries.end(), [](const CountryInfo& a, const CountryInfo& b)
	{
		// nProbes DESC
		if (a.nProbes != b.nProbes)
		{
			return a.nProbes > b.nProbes;
		}
		return false;
	});

	if (countries.empty())
	{
		return DEFAULT_PING_COUNTRY_CODE;
	}
	if (countries[0].nProbes < 1)
	{
		return DEFAULT_PING_COUNTRY_CODE;
	}

	return countries[0].sCode;
}

//------------------------------------------------------

int ListCountries(const CommonOptions& options)
{
	const int nWidth1 = 2;
	const int nWidth2 = 40;
	const int nWidth3 = 5;
	cout << setw(nWidth1) << "ID" << " " << setw(nWidth2) << left << "Country Name" << " " << setw(nWidth3) << "Number of hosts" << endl;
	cout << flush;

	ProbeApiRequester requester;

	ProbeApiRequester::Request request("GetCountries");

	const ProbeApiRequester::Reply reply = requester.DoRequest(request, options.bDebug);
	if (!reply.bSucceeded)
	{
		cerr << "ERROR! " << reply.sErrorDescription << endl;
		return eRetCode::ApiFailure;
	}

	cout << setfill('-') << setw(nWidth1 + 1 + nWidth2 + 1 + nWidth3 + 10) << "-" << setfill(' ') << endl;
	cout << flush;

	using namespace ProbeAPI;
	vector<CountryInfo> items;

	try
	{
		items = ParseCountries(reply.sBody);
	}
	catch (exception& e)
	{
		cerr << "ERROR! " << e.what() << endl;
		return eRetCode::ApiFailure;
	}

	// Sort data:
	sort(items.begin(), items.end(), [](const CountryInfo& a, const CountryInfo& b)
	{
		// nProbes DESC
		if (a.nProbes != b.nProbes)
		{
			return a.nProbes > b.nProbes;
		}
		// sCountryName ASC
		if (a.sName != b.sName)
		{
			return a.sName < b.sName;
		}
		return false;
	});

	// Print:
	for (const auto& info : items)
	{
		if (0 == info.nProbes)
		{
			//continue;
		}

		cout << setw(nWidth1) << info.sCode << " " << setw(nWidth2) << left << info.sName << right << " " << setw(nWidth3) << info.nProbes << endl;
	}

	return eRetCode::OK;
}

//------------------------------------------------------

struct MyAsnInfo
{
	ProbeAPI::ProbeInfo		probe;
	uint32_t				nAsnId = 0;
	int						nProbes = 0;
};

//------------------------------------------------------

int ListAsns(const CommonOptions& options)
{
	const string sCountryCode = options.sModeArgument;

	const int nWidth1 = 8;
	const int nWidth2 = 6;
	cout << setw(nWidth1) << left << "ASN id" << right << " " << setw(nWidth2) << "Hosts" << " " << "ASN name" << endl;
	cout << flush;

	ProbeApiRequester requester;

	const string sUrl = OSSFMT("GetProbesByCountry?countrycode=" << sCountryCode);

	ProbeApiRequester::Request request(sUrl);

	const ProbeApiRequester::Reply reply = requester.DoRequest(request, options.bDebug);
	if (!reply.bSucceeded)
	{
		cerr << "ERROR! " << reply.sErrorDescription << endl;
		return eRetCode::ApiFailure;
	}

	cout << setfill('-') << setw(nWidth1 + 1 + nWidth2 + 1 + 40) << "-" << setfill(' ') << endl;
	cout << flush;

	using namespace ProbeAPI;
	vector<ProbeInfo> items;

	try
	{
		items = ParseGetProbesByCountryResult_AsnOnly(reply.sBody);
	}
	catch (exception& e)
	{
		cerr << "ERROR! " << e.what() << endl;
		return eRetCode::ApiFailure;
	}

	// Deduplicate data:
	typedef decltype(MyAsnInfo().nAsnId) AsnId;
	map<AsnId, MyAsnInfo> items2;

	for (const auto& info : items)
	{
		AsnId id = 0;
		// Usually ASN id has a form "AS202732" where constant prefix "AS" is followed by integer:
		if (begins(info.asn.sId, "AS"))
		{
			id = stoul(info.asn.sId.substr(2));
		}
		else
		{
			hash<string> h1;
			id = h1(info.asn.sId);
		}

		auto& item = items2[id];
		item.probe = info;
		item.nAsnId = id;
		++item.nProbes;
	}

	// Sort data:
	items = vector<ProbeInfo>();	// free memory

	vector<MyAsnInfo> items3;
	items3.reserve(items2.size());

	for (const auto& p : items2)
	{
		items3.push_back(p.second);
	}

	items2.clear();	// free memory

	sort(items3.begin(), items3.end(), [](const MyAsnInfo& a, const MyAsnInfo& b)
	{
		// nProbes DESC
		if (a.nProbes != b.nProbes)
		{
			return a.nProbes > b.nProbes;
		}
		// nAsnId ASC
		if (a.nAsnId != b.nAsnId)
		{
			return a.nAsnId < b.nAsnId;
		}
		return false;
	});

	// Print:
	for (const auto& info : items3)
	{
		if (0 == info.nProbes)
		{
			//continue;
		}

		cout << setw(nWidth1) << left << info.probe.asn.sId << right << " " << setw(nWidth2) << info.nProbes << " " << info.probe.asn.sName << endl;
	}

	return eRetCode::OK;
}

//------------------------------------------------------
