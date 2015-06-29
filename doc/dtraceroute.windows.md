# dtraceroute

## About
`dtraceroute` is a tool for distributed trace route testing of specified destination host. This tool works almost the same as Windows and Linux built-in tools, but all network operations are going from different hosts in internet.

## Usage

```
Usage: dtracert --help
    --version
    --list-country [-v] [--debug]
    --list-asn code [-v] [--debug]
    --country code [-n probes] [-w timeout] [-h max_hops] [-v] {target_name}
    --asn id [-n probes] [-w timeout] [-h max_hops] [-v] {target_name}

Options:
    {target_name}  Destination host IP or domain name.

    --help          Display this help.
    --version       Display detailed program version, copyright notices.
    --list-country  List available countries.
    --list-asn code List ASNs for specified 2 letter country code.
    --country code  Specify source addresses 2 letter country code
                    (ISO 3166-1 alpha-2).
    --asn id        Use source addresses from specified ASN
                    (autonomous system number) network.
    -n probes       Probes limit: number of hosts to make requests from.
                    This option has an alias: --probes probes
    -w timeout      Timeout in milliseconds to wait for single ping.
    -h max_hops     Maximum number of hops to search for target (aka max TTL).
    -v              Verbose output.

Advanced options:
    -wa timeout     Timeout in milliseconds to wait for all probes.
    -d              Do not resolve addresses to hostnames.
    -hs hop_number  First hop number to start from.
    -hf max_hops    Maximum number of failed hops in a row to stop
                    (default: 3).
    -l size         Send buffer size.
    -f              Set Don't Fragment flag in packet (IPv4-only).
    -4              Force using IPv4.
    -6              Force using IPv6.
    --api-key key   Set web API key.
    --api-url url   Set web API URL.
    --no-delays     Disable delays during printing of results to console.
    --debug         Additional debug output.

Return Codes:
    0 - eRetCode::OK
   10 - eRetCode::BadArguments
   20 - eRetCode::NotSupported
   50 - eRetCode::Cancelled
  100 - eRetCode::ApiFailure
  110 - eRetCode::ApiParsingFail
  500 - eRetCode::OtherError
 1000 - eRetCode::HardFailure

Examples:
dtracert --list-country
dtracert --list-asn ES
dtracert --country US 8.8.8.8
dtracert --asn AS3352 8.8.8.8

```

### Example

#### --list-country
```
$ dtracert --list-country
ID Country Name                             Number of probes
-----------------------------------------------------------
RU Russian Federation                       18129
FR France                                    5365
DE Germany                                   5174
CZ Czech Republic                            2263
IT Italy                                     2240
HU Hungary                                   2140
ES Spain                                     2075
TR Turkey                                    1636
...
A2 Satellite Provider                           1
SC Seychelles                                   1
```

#### --list-asn
```
$ dtracert --list-asn ES
ASN id   Probes ASN name
--------------------------------------------------------
AS3352      838 TELEFONICA DE ESPANA
AS6739      285 Cableuropa - ONO
AS12479     276 France Telecom Espana SA
AS12715     257 Jazz Telecom S.A.
AS12357     143 VODAFONE ESPANA S.A.U.
AS12338      35 Euskaltel S.A.
AS12334      34 R Cable y Telecomunicaciones Galicia, S.A.
AS12430      32 VODAFONE ESPANA S.A.U.
...
AS199343      1 TV Loja Comunicaciones, S.L.
AS199435      1 Wimax On Line, S.L.
```

#### --country
```
$ dtracert --country US 8.8.8.8

Tracing route to 8.8.8.8 from country code US:
over a maximum of 30 hops:

Tracing route to google-public-dns-a.google.com [8.8.8.8] from probe ID 34805658 net "Optimum Online" (AS6128)
over a maximum of 30 hops:
  1    <1 ms     1 ms     1 ms  local.home [192.168.1.1]
  2     *        *        *     Request timed out.
  3    11 ms     9 ms     9 ms  ool-4353df19.dyn.optonline.net [67.83.223.25]
  4    22 ms    10 ms    13 ms  dstswr1-ge3-7.rh.rslyny.cv.net [167.206.34.9]
  5    13 ms    23 ms    10 ms  451be075.cst.lightpath.net [65.19.107.117]
  6    16 ms    14 ms    11 ms  451be0ce.cst.lightpath.net [65.19.120.206]
  7     *        *        *     72.14.215.203
  8     *        *        *     64.233.175.141
  9    12 ms    17 ms    14 ms  google-public-dns-a.google.com [8.8.8.8]

Trace complete.

Tracing route to google-public-dns-a.google.com [8.8.8.8] from probe ID 34764443 net "Time Warner Cable" (AS20001)
over a maximum of 30 hops:
...
...
...
over a maximum of 30 hops:
  1     2 ms     3 ms     1 ms  10.0.0.1
  2    10 ms     9 ms    15 ms  c-73-3-98-1.hsd1.ut.comcast.net [73.3.98.1]
  3     *        *        *     Request timed out.
  4    18 ms    13 ms    10 ms  he-0-18-0-0-ar01.saltlakecity.ut.utah.comcast.net [162.151.49.197]
  5    24 ms    23 ms    26 ms  be-33660-cr02.denver.co.ibone.comcast.net [68.86.90.225]
  6    22 ms    22 ms    23 ms  ae-17-0-pe01.910fifteenth.co.ibone.comcast.net [68.86.84.126]
  7    26 ms    23 ms    65 ms  as15169-1-c.910fifteenth.co.ibone.comcast.net [23.30.206.106]
  8    47 ms    24 ms    23 ms  216.239.42.249
  9     *        *        *     209.85.255.159
 10    40 ms    22 ms    22 ms  google-public-dns-a.google.com [8.8.8.8]

Trace complete.

```

#### --asn
```
$ dtracert --asn AS3320 8.8.8.8

Tracing route to 8.8.8.8 from AS3320:
over a maximum of 30 hops:

Tracing route to google-public-dns-a.google.com [8.8.8.8] from probe ID 34768618 net "Deutsche Telekom AG" (DE)
over a maximum of 30 hops:
  1     2 ms     1 ms     5 ms  Speedport.ip [192.168.2.1]
  2     *        *        *     217.0.118.228
  3    23 ms    26 ms    32 ms  87.186.233.2
  4     *        *        *     b-ea6-i.B.DE.NET.DTAG.DE [62.154.47.69]
  5     *        *        *     194.25.211.26
  6    55 ms    62 ms    52 ms  72.14.239.109
  7    27 ms    31 ms    27 ms  google-public-dns-a.google.com [8.8.8.8]

Trace complete.

Tracing route to google-public-dns-a.google.com [8.8.8.8] from probe ID 34595916 net "Deutsche Telekom AG" (DE)
over a maximum of 30 hops:
  1     6 ms     2 ms    21 ms  Speedport.ip [192.168.2.1]
  2     *        *        *     217.0.118.30
...
...
...

Tracing route to google-public-dns-a.google.com [8.8.8.8] from probe ID 34764142 net "Deutsche Telekom AG" (DE)
over a maximum of 30 hops:
  1     2 ms     1 ms     2 ms  speedport.ip [192.168.2.1]
  2     *        *        *     87.186.224.193
  3    27 ms    27 ms    27 ms  87.190.169.30
  4     *        *        *     217.239.41.94
  5    26 ms    23 ms    25 ms  72.14.217.76
  6    26 ms    25 ms    25 ms  209.85.248.107
  7     *        *        *     72.14.239.215
  8    25 ms    25 ms    26 ms  google-public-dns-a.google.com [8.8.8.8]

Trace complete.

```
