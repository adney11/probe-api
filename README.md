# ProbeAPI Tools
## About
`ProbeAPI Tools` is a set of cross platform console command line utilities which are using public API to make distributed pinging, tracerouting and etc.

## Usage

```
Usage: dping [--help]
             [--version]
             --list-country [-v] [--debug]
             --list-asn code [-v] [--debug]
             --country code [-n count] [-w timeout] [-v] [--debug] {target_name}
             --asn id [-n count] [-w timeout] [-v] [--debug] {target_name}

Options:
    {target_name}  Destination host IP or domain name.

    --help          Display this help.
    --version       Display detailed program version, copyright notices.
    --country code  Ping from specified 2 letter country code (ISO 3166-1 alpha-2).
    --asn id        Ping from specified ASN (autonomous system number) network.
    -n count        Number of echo requests to send.
    -w timeout      Timeout in milliseconds to wait for each reply.
    --list-country  List available countries.
    --list-asn code List ASNs for specified 2 letter country code.
    -v              Verbose output
    --debug         Additional debug output

Examples:
dping --list-country
dping --list-asn ES
dping --country US 8.8.8.8
dping --asn AS3352 8.8.8.8
```

### Example

#### --list-country
```
$ dping --list-country
ID Country Name                             Number of hosts
-----------------------------------------------------------
RU Russian Federation                       16231
DE Germany                                   5014
FR France                                    3932
CZ Czech Republic                            2034
UA Ukraine                                   1173
JP Japan                                     1121
...
TG Togo                                         1
TT Trinidad and Tobago                          1
```

#### --list-asn
```
$ dping --list-asn ES
ASN id   Probes ASN name
--------------------------------------------------------
AS174         1 Cogent Communications
AS766         2 Entidad Publica Empresarial Red.es
AS3352      149 TELEFONICA DE ESPANA
AS6739       61 Cableuropa - ONO
...
AS62332       2 Telplay SL
AS197100      1 Esystel Servicios Multimedia, SL
AS197722      1 Electrovideo Utrera S.A.
```

#### --country
```
$ dping --country US 8.8.8.8

Pinging 8.8.8.8 with 32 bytes of data from country code US:
Reply from 8.8.8.8: bytes=32 time=99ms TTL=128
Reply from 8.8.8.8: bytes=32 time=173ms TTL=128
Reply from 8.8.8.8: bytes=32 time=18ms TTL=128
Reply from 8.8.8.8: bytes=32 time=32ms TTL=128
Reply from 8.8.8.8: bytes=32 time=27ms TTL=128
Reply from 8.8.8.8: bytes=32 time=24ms TTL=128
Reply from 8.8.8.8: bytes=32 time=18ms TTL=128

Ping statistics for 8.8.8.8
    Packets : Sent = 7, Received = 7, Lost = 0 (0 % loss),
Approximate round trip times in milli-seconds:
    Minimum = 18ms, Maximum = 173ms, Average = 55ms
```

#### --asn
```
$ dping --asn AS3352 8.8.8.8

Pinging 8.8.8.8 with 32 bytes of data from AS3352:
Reply from 8.8.8.8: bytes=32 time=56ms TTL=128
Reply from 8.8.8.8: bytes=32 time=43ms TTL=128
Reply from 8.8.8.8: bytes=32 time=38ms TTL=128
Reply from 8.8.8.8: bytes=32 time=61ms TTL=128
Reply from 8.8.8.8: bytes=32 time=70ms TTL=128
Reply from 8.8.8.8: bytes=32 time=60ms TTL=128
Reply from 8.8.8.8: bytes=32 time=9ms TTL=128
Reply from 8.8.8.8: bytes=32 time=50ms TTL=128

Ping statistics for 8.8.8.8
    Packets : Sent = 8, Received = 8, Lost = 0 (0 % loss),
Approximate round trip times in milli-seconds:
    Minimum = 9ms, Maximum = 70ms, Average = 48ms
```

## Compilation

Microsoft Visual Studio 2013 is currently the only way to compile. Solution and a set of project files are provided in Git repository.
There is a number of external dependencies in "deps" subfolder. Please, follow \_README\_ files to find and place correspong project sources:
* /deps/curl/\_README\_ 
* /deps/curlpp/\_README\_ 
* /deps/jsoncpp/\_README\_ 

## Installation

No special installation is needed. Currently precompiled binary EXE file is provided and it does not need any installation. It is fully portable and it has no external dependencies.

## Credits

```
ProbeAPI Tools: dping
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
