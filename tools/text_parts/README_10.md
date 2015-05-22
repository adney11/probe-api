# ProbeAPI Tools
## About
`ProbeAPI Tools` is a set of cross platform console command line utilities which are using public API to make distributed pinging, tracerouting and etc.
All these tools are working almost the same as Windows and Linux built-in tools, but all network operations are going from different hosts in internet.
"d" prefix from the name of all tools comes from thw word **d**istributed.

## Particular Tools Documentation

* [d**ping**](dping.md)
* [d**traceroute** (d**tracert**)](dtraceroute.md)

## Compilation

Microsoft Visual Studio 2013 is currently the only way to compile. Solution and a set of project files are provided in Git repository.
There is a number of external dependencies in "deps" subfolder. Please, follow \_README\_ files to find and place correspong project sources:
* /deps/curl/\_README\_ 
* /deps/curlpp/\_README\_ 
* /deps/jsoncpp/\_README\_ 

## Installation

No special installation is needed for any of the tools. Currently precompiled binary EXE files are provided and they do not need any installation. Executables are fully portable and they have no external dependencies.

## Credits

```
