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
Copyright (c) 2015 Optimal Software s.r.o. All rights reserved.

Sources repository:  https://github.com/optimal-software/probe-api
Latest license:      https://github.com/optimal-software/probe-api/blob/master/LICENSE

Authors: Sergey Kolomenkin

===============================================================================

The MIT License (MIT)

Copyright (c) 2015 ProbeAPI Tools

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

===============================================================================

Used third-party libraries:
cURL        : libcurl/7.42.1 WinSSL; Protocols: http https
    Website : http://curl.haxx.se/
    Repo    : https://github.com/bagder/curl
    License : MIT; http://curl.haxx.se/docs/copyright.html
cURLpp      : 0.7.3
    Website : http://rrette.com/curlpp.html
    Repo    : https://github.com/jpbarrette/curlpp
    License : MIT; http://www.curlpp.org/#license
jsoncpp     : 0.10.2
    Website : https://github.com/open-source-parsers/jsoncpp
    Repo    : https://github.com/open-source-parsers/jsoncpp
    License : Public Domain, MIT; https://github.com/open-source-parsers/jsoncpp/blob/master/LICENSE
```
