# dtraceroute

## About
`dtraceroute` is a tool which works almost the same as Windows and Linux built-in tool, but all network operations are going from different hosts in internet.

## Usage

```
Usage: dtracert --help
    --version
    --list-country [-v] [--debug]
    --list-asn code [-v] [--debug]
    --country code [-n count] [-w timeout] [-h maximum_hops] [-v] [--debug] {target_name}
    --asn id [-n count] [-w timeout] [-h maximum_hops] [-v] [--debug] {target_name}

Options:
    {target_name}  Destination host IP or domain name.

    --help          Display this help.
    --version       Display detailed program version, copyright notices.
    --country code  Specify source addresses 2 letter country code (ISO 3166-1 alpha-2).
    --asn id        Use source addresses from specified ASN (autonomous system number) network.
    -n count        Number of echo requests to send.
    -w timeout      Timeout in milliseconds to wait for each reply.
    -h maximum_hops Maximum number of hops to search for target.
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
ID Country Name                             Number of hosts
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
ASN id    Hosts ASN name
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

  1     2 ms     1 ms     1 ms  10.0.0.1
  2    12 ms    12 ms    11 ms  96.120.16.161
  3    12 ms    25 ms    18 ms  68.85.255.9
  4    20 ms    12 ms    11 ms  68.85.244.173
...
...
...
  6    16 ms    20 ms    15 ms  65.29.1.87
  7    18 ms    20 ms    20 ms  65.29.1.212
  8    30 ms    33 ms    31 ms  107.14.19.36
  9    52 ms    53 ms    63 ms  66.109.9.41
 10    49 ms    52 ms    50 ms  107.14.17.232
 11    86 ms    68 ms    69 ms  207.86.210.125
 12     *        *        *     207.88.14.182
 13    67 ms    69 ms    68 ms  207.88.14.189
 14    68 ms    66 ms    68 ms  65.47.204.58
 15    68 ms    67 ms    68 ms  216.239.47.84
 16    66 ms    69 ms    67 ms  8.8.8.8

Trace complete.

```

#### --asn
```
$ dtracert --asn AS3320 8.8.8.8

Tracing route to 8.8.8.8 from AS3320:
over a maximum of 30 hops:

  1    <1 ms    <1 ms    <1 ms  172.16.2.254
  2     *        *        *     62.159.95.221
  3     3 ms    16 ms     3 ms  62.154.10.157
  4     1 ms    12 ms     1 ms  217.239.41.222
  5     2 ms     1 ms     1 ms  74.125.50.149
  6     1 ms     1 ms     1 ms  64.233.175.121
  7     1 ms     1 ms     1 ms  8.8.8.8

Trace complete.

  1    <1 ms    <1 ms    <1 ms  192.168.2.1
  2     *        *        *     87.186.224.50
  3    21 ms    22 ms    22 ms  87.190.169.42
  4    25 ms    25 ms    24 ms  62.154.15.2
  5    25 ms    25 ms    25 ms  72.14.217.76
  6    25 ms    57 ms    26 ms  209.85.248.107
...
...
...
  2     *        *        *     217.0.119.81
  3    22 ms    19 ms    19 ms  217.0.65.222
  4    23 ms    23 ms    23 ms  194.25.6.74
  5    29 ms    24 ms    29 ms  72.14.217.76
  6    24 ms   115 ms    24 ms  209.85.248.27
  7     *        *        *     216.239.46.177
  8    24 ms    23 ms    25 ms  8.8.8.8

Trace complete.

  1    47 ms    47 ms    47 ms  8.8.8.8

Trace complete.

```
