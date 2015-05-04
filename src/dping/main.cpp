//------------------------------------------------------

#include "stdafx.h"

#include <json\json.h>

#include <iostream>

using namespace std;

//------------------------------------------------------

int _tmain(int argc, _TCHAR* argv[])
{
	const string config_doc = R"zzz(
// Configuration options
{
    // Default encoding for text
    "encoding" : "UTF-8",
    
    // Plug-ins loaded at start-up
    "plug-ins" : [
        "python",
        "c++",
        "ruby"
        ],
        
    // Tab indent size
    "indent" : { "length" : 3, "use_space": true }
}
)zzz";

	Json::Value root;   // will contains the root value after parsing.
	Json::Reader reader;
	bool parsingSuccessful = reader.parse(config_doc, root);
	if (!parsingSuccessful)
	{
		// report to the user the failure and their locations in the document.
		cout << "Failed to parse configuration\n"
			<< reader.getFormattedErrorMessages();
		return 0;
	}

	// Get the value of the member of root named 'encoding', return 'UTF-8' if there is no
	// such member.
	const string encoding = root.get("encoding", "UTF-8").asString();
	// Get the value of the member of root named 'encoding', return a 'null' value if
	// there is no such member.
	const Json::Value plugins = root["plug-ins"];
	for (size_t index = 0; index < plugins.size(); ++index)  // Iterates over the sequence elements.
	{
		cout << "plugin: " << plugins[index].asString() << endl;
	}

	Json::StyledWriter writer;
	// Make a new JSON document for the configuration. Preserve original comments.
	const string outputConfig = writer.write(root);

	// And you can write to a stream, using the StyledWriter automatically.
	cout << root["plug-ins"] << endl;

	return 0;
}

//------------------------------------------------------
