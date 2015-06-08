# dping

## About
`dping` is a tool for distributed pinging of specified destination host. This tool works almost the same as Windows and Linux built-in tools, but all network operations are going from different hosts in internet.

## Usage

```
Usage: dping --help
    --version
    --list-country [-v] [--debug]
    --list-asn code [-v] [--debug]
    --country code [-n count] [-w timeout] [-wa timeout] [--no-delays] [-v]
                   [--debug] {target_name}
    --asn id [-n count] [-w timeout] [-wa timeout] [--no-delays] [-v] [--debug]
             {target_name}

Options:
    {target_name}  Destination host IP or domain name.

    --help          Display this help.
    --version       Display detailed program version, copyright notices.
    --country code  Specify source addresses 2 letter country code
                    (ISO 3166-1 alpha-2).
    --asn id        Use source addresses from specified ASN
                    (autonomous system number) network.
    -n count        Number of probes (hosts to make network requests from).
    -w timeout      Timeout in milliseconds to wait for single ping.
    -wa timeout     Timeout in milliseconds to wait for all probes.
    --list-country  List available countries.
    --list-asn code List ASNs for specified 2 letter country code.
    --no-delays     Disable delays during printing of results to console.
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
dping --list-country
dping --list-asn ES
dping --country US 8.8.8.8
dping --asn AS3352 8.8.8.8

```

### Example

#### --list-country
```
$ dping --list-country
ID Country Name                             Number of probes
-----------------------------------------------------------
RU Russian Federation                       18064
FR France                                    5337
DE Germany                                   5157
CZ Czech Republic                            2247
IT Italy                                     2232
HU Hungary                                   2134
ES Spain                                     2065
TR Turkey                                    1627
...
A2 Satellite Provider                           1
SC Seychelles                                   1
```

#### --list-asn
```
$ dping --list-asn ES
ASN id   Probes ASN name
--------------------------------------------------------
AS3352      836 TELEFONICA DE ESPANA
AS6739      282 Cableuropa - ONO
AS12479     275 France Telecom Espana SA
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
$ dping --country US 8.8.8.8
PING 8.8.8.8 (8.8.8.8) 32(60) bytes of data from country code US.
32 bytes from 8.8.8.8: icmp_seq=1 ttl=128 time=30.0 ms
32 bytes from 8.8.8.8: icmp_seq=1 ttl=128 time=35.0 ms
32 bytes from 8.8.8.8: icmp_seq=1 ttl=128 time=666.0 ms
32 bytes from 8.8.8.8: icmp_seq=1 ttl=128 time=62.0 ms
--- 8.8.8.8 ping statistics ---
10 packets transmitted, 4 received, 60% packet loss, time 7103ms
rtt min/avg/max/mdev = 30.000/198.250/666.000/16.000 ms
```

#### --asn
```
$ dping --asn AS3352 8.8.8.8
PING 8.8.8.8 (8.8.8.8) 32(60) bytes of data from AS3352.
32 bytes from 8.8.8.8: icmp_seq=1 ttl=128 time=81.0 ms
32 bytes from 8.8.8.8: icmp_seq=1 ttl=128 time=30.0 ms
32 bytes from 8.8.8.8: icmp_seq=1 ttl=128 time=35.0 ms
32 bytes from 8.8.8.8: icmp_seq=1 ttl=128 time=666.0 ms
32 bytes from 8.8.8.8: icmp_seq=1 ttl=128 time=13.0 ms
32 bytes from 8.8.8.8: icmp_seq=1 ttl=128 time=40.0 ms
32 bytes from 8.8.8.8: icmp_seq=1 ttl=128 time=62.0 ms
--- 8.8.8.8 ping statistics ---
10 packets transmitted, 7 received, 30% packet loss, time 5978ms
rtt min/avg/max/mdev = 13.000/132.429/666.000/22.000 ms
```
