# ProbeAPI Tools
## About
`ProbeAPI Tools` is a set of cross platform console command line utilities which are using public API to make distributed pinging, tracerouting and etc.
All these tools are working almost the same as Windows and Linux built-in tools, but all network operations are going from different hosts in internet.
"d" prefix from the name of all tools comes from the word **d**istributed.

## Particular Tools Documentation

### Windows
* [d**ping**](doc/dping.windows.md)
* [d**tracert**](doc/dtraceroute.windows.md)

### Linux
* [d**ping**](doc/dping.linux.md)
* [d**traceroute**](doc/dtraceroute.linux.md)

## Compilation

Two main ways to build tools are supported:
* Microsoft Visual Studio 2013 solution
* nmake config (CMakeLists.txt)

Tested environments and compilers:
* Microsoft Visual Studio 2013 (2012 should be OK too, but not 2010)
* MinGW for Windows (with gcc 4.8.1)
* gcc 4.9.2 (tested in Ubuntu 15.04)

### Compilation in Linux
In Linux you will need to install some packages prior to building projects. Please run the following commands to install required packages:
```
sudo apt-get install git cmake libcurl4-openssl-dev make gcc
```
Installing optional packages (needed in case of building documentation, building and installing RPM):
```
sudo apt-get install librpmbuild3 alien bash gawk
```

Checkout and build ProbeAPI Tools:
```
cd ~
git clone --recursive https://github.com/optimal-software/probe-api.git
cd probe-api/build
cmake ..
make
```

Now you can run tools:
```
cd ~/probe-api/bin
./dping --version
./dtraceroute --version
```

You can also build RPM:
```
cd ~/probe-api/build
make package
```

### Compilation in Windows

Please checkout repository recursively:
```
git clone --recursive https://github.com/optimal-software/probe-api.git
```

#### MinGW
1. Please install cmake for Windows.
2. Execute following commands:
```
cd probe-api/build
```
And later:
```
cmake -G "MinGW Makefiles" ..
make
```
or
```
cmake ..
make
```

#### Microsoft Visual Studio

Microsoft Visual Studio solution file is provided in the root of Git repository.

## Installation

No special installation is needed for any of the tools. Currently precompiled binary EXE files are provided and they do not need any installation. Executables are fully portable and they have no external dependencies.
Pre-built 64 bit RPM is also provided for Linux operation systems. It may be installed by command:
```
sudo alien -i probe-api-tools-******.x86_64.rpm
```
or
```
sudo rpm -i probe-api-tools-******.x86_64.rpm
```
(please replace "******" by corresponding product version from RPM file name)

## Credits

```
Copyright (c) 2015 Optimal Software s.r.o. All rights reserved.

Sources repository:  https://github.com/optimal-software/probe-api
Latest license:      https://github.com/optimal-software/probe-api/blob/master/LICENSE

Build mode: Release

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
cURL        : libcurl/7.42.1 WinSSL zlib/1.2.8; Protocols: http https
    Website : http://curl.haxx.se/
    Repo    : https://github.com/bagder/curl.git
    License : MIT; http://curl.haxx.se/docs/copyright.html
  zlib
    Website : http://zlib.net/
    Repo    : https://github.com/madler/zlib.git
    License : MIT; http://zlib.net/zlib_license.html
cURLpp      : 0.7.3
    Website : http://rrette.com/curlpp.html
    Repo    : https://github.com/jpbarrette/curlpp.git
    License : MIT; http://www.curlpp.org/#license
JsonCpp     : 0.10.2
    Website : https://github.com/open-source-parsers/jsoncpp
    Repo    : https://github.com/open-source-parsers/jsoncpp.git
    License : Public Domain, MIT; https://github.com/open-source-parsers/jsoncpp/blob/master/LICENSE
```
