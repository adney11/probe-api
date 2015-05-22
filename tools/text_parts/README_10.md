# ProbeAPI Tools
## About
`ProbeAPI Tools` is a set of cross platform console command line utilities which are using public API to make distributed pinging, tracerouting and etc.
All these tools are working almost the same as Windows and Linux built-in tools, but all network operations are going from different hosts in internet.
"d" prefix from the name of all tools comes from the word **d**istributed.

## Particular Tools Documentation

* [d**ping**](dping.md)
* [d**traceroute** (d**tracert**)](dtraceroute.md)

## Compilation

Microsoft Visual Studio 2013 is currently the only way to compile ProbeAPI Tools. Solution and a set of project files are provided in Git repository.
There is a number of external dependencies in "deps" subfolder. Please, follow hints in \_README\_ files to download and unpack correspong project sources:
* [cURL readme](/deps/curl/_README_)
* [cURLpp readme](/deps/curlpp/_README_)
* [JsonCpp readme](/deps/jsoncpp/_README_)

## Installation

No special installation is needed for any of the tools. Currently precompiled binary EXE files are provided and they do not need any installation. Executables are fully portable and they have no external dependencies.

## Credits

```
