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

In linux you will need to install some packets prior to building projects. Please run the following commands:
Required packets:
```
sudo apt-get install libcurl4-openssl-dev cmake git gcc make
```
Optional packets:
```
sudo apt-get install librpmbuild3 alien zlib1g-dev libssl-dev
```

Microsoft Visual Studio Solution and a set of project files are provided in Git repository.

There is a number of external dependencies in "deps" subfolder. Please use git commands to checkout repository recursively, so you will get all necessary external sources automatically.

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
