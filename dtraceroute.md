# dtraceroute

## About
`dtraceroute` is a tool which works almost the same as Windows and Linux built-in tool, but all network operations are going from different hosts in internet.

## Usage

```
Usage: dtracert --help
    --version
    --list-country [-v] [--debug]
    --list-asn code [-v] [--debug]
    --country code [-n count] [-w timeout] [-h max_hops] [-hf max_hops]
                   [-wa timeout] [-v] [--debug] {target_name}
    --asn id [-n count] [-w timeout] [-h max_hops] [-hf max_hops]
             [-wa timeout] [-v] [--debug] {target_name}

Options:
    {target_name}  Destination host IP or domain name.

    --help          Display this help.
    --version       Display detailed program version, copyright notices.
    --country code  Specify source addresses 2 letter country code
                    (ISO 3166-1 alpha-2).
    --asn id        Use source addresses from specified ASN
                    (autonomous system number) network.
    -n count        Number of probes: hosts to make network requests from.
    -w timeout      Timeout in milliseconds to wait for single ping.
    -wa timeout     Timeout in milliseconds to wait for all probes.
    -h max_hops     Maximum number of hops to search for target (aka TTL).
    -hf max_hops    Maximum number of failed hops in a row to stop.
    --list-country  List available countries.
    --list-asn code List ASNs for specified 2 letter country code.
    -v              Verbose output
    --debug         Additional debug output

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

Tracing route to [8.8.8.8] from probe ID 7718232 net "Comcast Cable" (AS7922)
over a maximum of 30 hops:
  1    <1 ms    <1 ms    <1 ms  10.0.0.1
  2     6 ms     8 ms     7 ms  50.54.32.1
  3     7 ms     8 ms     7 ms  74.42.150.33
  4    16 ms    19 ms    17 ms  74.42.149.189
  5    18 ms    16 ms    16 ms  74.40.4.138
  6    16 ms    17 ms    16 ms  72.14.212.143
  7    17 ms    17 ms    17 ms  209.85.143.135
  8    48 ms    17 ms    17 ms  216.239.40.87
  9    17 ms    17 ms    17 ms  8.8.8.8

Trace complete.

Tracing route to [8.8.8.8] from probe ID 7692964 net "CenturyLink" (AS209)
over a maximum of 30 hops:
...
...
...
Tracing route to [8.8.8.8] from probe ID 6866195 net "Comcast Cable" (AS7922)
over a maximum of 30 hops:
  1     3 ms     2 ms     2 ms  10.0.0.1
  2     9 ms     9 ms     8 ms  68.39.72.1
  3     9 ms     9 ms     9 ms  162.151.35.101
  4    14 ms    12 ms    19 ms  68.87.231.73
  5    23 ms    24 ms    28 ms  68.87.230.9
  6    22 ms    24 ms    20 ms  4.68.71.166
  7     *        *        *     216.239.43.99
  8     *        *        *     209.85.142.255
  9    20 ms    20 ms    23 ms  8.8.8.8

Trace complete.

```

#### --asn
```
$ dtracert --asn AS3320 8.8.8.8

Tracing route to 8.8.8.8 from AS3320:
over a maximum of 30 hops:

Tracing route to [8.8.8.8] from probe ID 7722387 net "Deutsche Telekom AG" (DE)
over a maximum of 30 hops:
  1    <1 ms    <1 ms    <1 ms  172.16.2.254
  2     *        *        *     62.159.95.221
  3     3 ms    16 ms     3 ms  62.154.10.157
  4     1 ms    12 ms     1 ms  217.239.41.222
  5     2 ms     1 ms     1 ms  74.125.50.149
  6     1 ms     1 ms     1 ms  64.233.175.121
  7     1 ms     1 ms     1 ms  8.8.8.8

Trace complete.

Tracing route to [8.8.8.8] from probe ID 6959253 net "Deutsche Telekom AG" (DE)
over a maximum of 30 hops:
  1     2 ms     1 ms     1 ms  192.168.2.1
  2     *        *        *     217.0.117.240
...
...
...

Tracing route to [8.8.8.8] from probe ID 7676323 net "Deutsche Telekom AG" (DE)
over a maximum of 30 hops:
  1    <1 ms    <1 ms    <1 ms  192.168.2.1
  2     *        *        *     217.0.117.172
  3    23 ms    23 ms    24 ms  87.186.196.226
  4    26 ms    22 ms    22 ms  217.239.48.202
  5    22 ms    22 ms    22 ms  72.14.217.76
  6    23 ms    23 ms    23 ms  209.85.247.209
  7     *        *        *     209.85.246.191
  8    23 ms    75 ms    23 ms  8.8.8.8

Trace complete.

```
