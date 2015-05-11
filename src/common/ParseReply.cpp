//------------------------------------------------------
#include "stdafx.h"
#include "ParseReply.h"

//------------------------------------------------------

#ifdef _MSC_VER
#pragma comment (lib, "jsoncpp.lib")
#endif // _MSC_VER > 1000

//------------------------------------------------------

#if 0
reply HTTP code: 401 Unauthorized
Content-Type: application/json
{"message":"Missing Mashape application key. Go to http:\/\/docs.mashape.com\/api-keys to learn how to get your API application key."}

reply HTTP code: 402
Content - Type : application / json
{"message":"You need to subscribe to a plan before consuming the API"}

reply HTTP code : 403
Content - Type : application / json
{"message":"Invalid Mashape Key"}

reply HTTP code: 404
Content-Type: text/html; charset=UTF-8
<body>
<div id="content">
<p class="heading1">Service</p>
<p>Endpoint not found.</p>
</div>
</body>
#endif

//------------------------------------------------------

#if 0
//#include <json/json.h>

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
	return 1;
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
#endif

//------------------------------------------------------
