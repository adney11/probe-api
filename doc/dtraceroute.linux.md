# dtraceroute

## About
`dtraceroute` is a tool for distributed trace route testing of specified destination host. This tool works almost the same as Windows and Linux built-in tools, but all network operations are going from different hosts in internet.

## Usage

```
Usage: dtraceroute --help
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
dtraceroute --list-country
dtraceroute --list-asn ES
dtraceroute --country US 8.8.8.8
dtraceroute --asn AS3352 8.8.8.8

```

### Example

#### --list-country
```
$ dtraceroute --list-country
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
$ dtraceroute --list-asn ES
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
$ dtraceroute --country US 8.8.8.8
traceroute to 8.8.8.8 (8.8.8.8) from country code US, 30 hops max, 32 byte packets
traceroute to google-public-dns-a.google.com (8.8.8.8) (8.8.8.8) from probe ID 34748172 net "Comcast Cable" (AS7922), 30 hops max, 32 byte packets
 1   (10.0.0.1)  2.000 ms  1.000 ms  2.000 ms
 2   (96.120.25.33)  9.000 ms  11.000 ms  14.000 ms
 3  te-0-4-0-18-sur04.chicago302.il.chicago.comcast.net (68.86.116.13)  10.000 ms  12.000 ms  11.000 ms
 4  te-1-12-0-8-ar01.area4.il.chicago.comcast.net (69.139.202.253)  28.000 ms  11.000 ms  13.000 ms
 5  be-33491-cr01.350ecermak.il.ibone.comcast.net (68.86.91.165)  13.000 ms  19.000 ms  12.000 ms
 6  he-0-10-0-0-pe04.350ecermak.il.ibone.comcast.net (68.86.83.50)  12.000 ms  10.000 ms  11.000 ms
 7  as15169-2-c.350ecermak.il.ibone.comcast.net (66.208.233.142)  21.000 ms  11.000 ms  12.000 ms
 8   (216.239.43.117) * * *
 9   (216.239.50.5) * * *
10  google-public-dns-a.google.com (8.8.8.8)  17.000 ms  10.000 ms  12.000 ms
traceroute to google-public-dns-a.google.com (8.8.8.8) (8.8.8.8) from probe ID 34703767 net "Optimum Online" (AS6128), 30 hops max, 32 byte packets
 1  openrg.home (192.168.1.1)  0.000 ms  1.000 ms  1.000 ms
 2   () * * *
 3   (67.59.232.73)  10.000 ms  8.000 ms  8.000 ms
 4  rtr4-ge1-12.mhe.prnynj.cv.net (67.83.242.133)  10.000 ms  10.000 ms  10.000 ms
 5   (64.15.7.69)  10.000 ms  11.000 ms  10.000 ms
 6  451be0c6.cst.lightpath.net (65.19.120.198)  15.000 ms  13.000 ms  16.000 ms
 7   (72.14.215.203)  36.000 ms  14.000 ms  12.000 ms
...
...
...
10   (209.85.250.70) * * *
11   (209.85.251.243) * * *
12   () * * *
13  google-public-dns-a.google.com (8.8.8.8)  24.000 ms  90.000 ms  106.000 ms
traceroute to google-public-dns-a.google.com (8.8.8.8) (8.8.8.8) from probe ID 34806822 net "AT&T U-verse" (AS7018), 30 hops max, 32 byte packets
 1  homeportal (192.168.1.254)  3.000 ms  1.000 ms  3.000 ms
 2   () * * *
 3   (75.19.192.204)  26.000 ms  26.000 ms  24.000 ms
 4   (12.83.79.5) * * *
 5  ggr2.cgcil.ip.att.net (12.122.132.137) * * *
 6   () * * *
 7   (216.239.43.99)  38.000 ms  36.000 ms  36.000 ms
 8   (216.239.50.71) * * *
 9  google-public-dns-a.google.com (8.8.8.8)  35.000 ms  37.000 ms  37.000 ms
```

#### --asn
```
$ dtraceroute --asn AS3320 8.8.8.8
traceroute to 8.8.8.8 (8.8.8.8) from AS3320, 30 hops max, 32 byte packets
traceroute to google-public-dns-a.google.com (8.8.8.8) (8.8.8.8) from probe ID 34752278 net "Deutsche Telekom AG" (DE), 30 hops max, 32 byte packets
 1  speedport.ip (192.168.2.1)  1.000 ms  1.000 ms  2.000 ms
 2   (217.0.118.125) * * *
 3   (87.186.246.6)  148.000 ms  11.000 ms  72.000 ms
 4  b-ea6-i.B.DE.NET.DTAG.DE (62.154.47.66) * * *
 5   (194.25.211.26) * * *
 6   (72.14.234.223)  22.000 ms  109.000 ms  100.000 ms
 7  google-public-dns-a.google.com (8.8.8.8)  79.000 ms  37.000 ms  80.000 ms
traceroute to google-public-dns-a.google.com (8.8.8.8) (8.8.8.8) from probe ID 34684806 net "Deutsche Telekom AG" (DE), 30 hops max, 32 byte packets
 1  fritz.box (192.168.178.1)  1.000 ms  3.000 ms  1.000 ms
 2   (217.0.119.183) * * *
 3   (217.0.75.82)  369.000 ms  210.000 ms  170.000 ms
 4  b-ea6-i.B.DE.NET.DTAG.DE (62.154.47.69) * * *
 5   (194.25.211.26) * * *
 6   (72.14.234.227)  93.000 ms  203.000 ms  175.000 ms
 7  google-public-dns-a.google.com (8.8.8.8)  30.000 ms  29.000 ms  208.000 ms
traceroute to google-public-dns-a.google.com (8.8.8.8) (8.8.8.8) from probe ID 31870352 net "Deutsche Telekom AG" (DE), 30 hops max, 32 byte packets
 1  speedport.ip (192.168.2.1)  1.000 ms  1.000 ms  1.000 ms
 2   (87.186.224.131) * * *
...
...
...
 7   (209.85.143.77) * * *
 8   (209.85.255.85) * * *
 9   (209.85.255.49) * * *
10   () * * *
11  google-public-dns-a.google.com (8.8.8.8)  31.000 ms  26.000 ms  25.000 ms
traceroute to google-public-dns-a.google.com (8.8.8.8) (8.8.8.8) from probe ID 34804841 net "Deutsche Telekom AG" (DE), 30 hops max, 32 byte packets
 1  speedport.ip (192.168.2.1)  0.000 ms  0.000 ms  0.000 ms
 2   (87.186.224.69) * * *
 3   (217.0.75.78)  19.000 ms  20.000 ms  19.000 ms
 4   (194.25.6.74) * * *
 5   (72.14.217.76) * * *
 6   (209.85.248.27)  19.000 ms  20.000 ms  96.000 ms
 7   (209.85.241.41) * * *
 8  google-public-dns-a.google.com (8.8.8.8)  20.000 ms  20.000 ms  20.000 ms
```
