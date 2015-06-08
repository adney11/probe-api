# dtraceroute

## About
`dtraceroute` is a tool for distributed trace route testing of specified destination host. This tool works almost the same as Windows and Linux built-in tools, but all network operations are going from different hosts in internet.

## Usage

```
Usage: dtraceroute --help
    --version
    --list-country [-v] [--debug]
    --list-asn code [-v] [--debug]
    --country code [-n probes] [-w timeout] [-h max_hops] [-hf max_hops]
                   [-wa timeout] [--no-delays] [-v] [--debug] {target_name}
    --asn id [-n probes] [-w timeout] [-h max_hops] [-hf max_hops]
             [-wa timeout] [--no-delays] [-v] [--debug] {target_name}

Options:
    {target_name}  Destination host IP or domain name.

    --help          Display this help.
    --version       Display detailed program version, copyright notices.
    --country code  Specify source addresses 2 letter country code
                    (ISO 3166-1 alpha-2).
    --asn id        Use source addresses from specified ASN
                    (autonomous system number) network.
    -n probes       Probes limit: number of hosts to make requests from.
    -w timeout      Timeout in milliseconds to wait for single ping.
    -wa timeout     Timeout in milliseconds to wait for all probes.
    -h max_hops     Maximum number of hops to search for target (aka TTL).
    -hf max_hops    Maximum number of failed hops in a row to stop.
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
$ traceroute --country US 8.8.8.8
traceroute to 8.8.8.8 (8.8.8.8) from country code US, 30 hops max, 32 byte packets
traceroute to 8.8.8.8 (8.8.8.8) from probe ID 14786136 net "Telscape Communications" (AS6302), 30 hops max, 32 byte packets
 1  192.168.1.1 (192.168.1.1)  0.000 ms  4.000 ms  1.000 ms
 2  209.127.214.1 (209.127.214.1)  111.000 ms  141.000 ms  13.000 ms
 3  172.16.3.18 (172.16.3.18)  13.000 ms  38.000 ms  37.000 ms
traceroute to 8.8.8.8 (8.8.8.8) from probe ID 16949717 net "Cox Communications" (AS22773), 30 hops max, 32 byte packets
 1  192.168.0.1 (192.168.0.1)  0.000 ms  0.000 ms  0.000 ms
 2  172.21.0.60 (172.21.0.60)  8.000 ms  7.000 ms  7.000 ms
 3  70.169.77.216 (70.169.77.216)  10.000 ms  8.000 ms  7.000 ms
 4  68.1.0.151 (68.1.0.151)  21.000 ms  22.000 ms  21.000 ms
 5  72.14.215.221 (72.14.215.221)  20.000 ms  23.000 ms  22.000 ms
 6  209.85.240.183 (209.85.240.183)  21.000 ms  22.000 ms  22.000 ms
 7  8.8.8.8 (8.8.8.8)  21.000 ms  21.000 ms  20.000 ms
traceroute to 8.8.8.8 (8.8.8.8) from probe ID 17183468 net "Comcast Cable" (AS7922), 30 hops max, 32 byte packets
 1  192.168.1.1 (192.168.1.1)  1.000 ms  1.000 ms  1.000 ms
 2  96.120.15.37 (96.120.15.37)  14.000 ms  41.000 ms  26.000 ms
 3  68.86.112.221 (68.86.112.221)  38.000 ms  18.000 ms  77.000 ms
 4  68.86.241.109 (68.86.241.109)  85.000 ms  72.000 ms  64.000 ms
 5  68.86.93.93 (68.86.93.93)  56.000 ms  25.000 ms  25.000 ms
 6  68.86.82.134 (68.86.82.134)  26.000 ms  25.000 ms  28.000 ms
...
...
...
 7  96.34.1.149 (96.34.1.149)  68.000 ms  41.000 ms  41.000 ms
 8  96.34.3.11 (96.34.3.11)  40.000 ms  33.000 ms  36.000 ms
 9  96.34.152.30 (96.34.152.30) * * *
10  216.239.43.111 (216.239.43.111)  50.000 ms  47.000 ms  45.000 ms
11  216.239.50.123 (216.239.50.123) * * *
12  8.8.8.8 (8.8.8.8)  34.000 ms  34.000 ms  33.000 ms
traceroute to 8.8.8.8 (8.8.8.8) from probe ID 17195058 net "AT&T U-verse" (AS7018), 30 hops max, 32 byte packets
 1  192.168.1.254 (192.168.1.254)  0.000 ms  0.000 ms  0.000 ms
 2  172.4.148.2 (172.4.148.2)  39.000 ms  19.000 ms  19.000 ms
 3  71.145.0.192 (71.145.0.192)  20.000 ms  19.000 ms  19.000 ms
 4  12.83.39.185 (12.83.39.185) * * *
 5  12.122.136.181 (12.122.136.181) * * *
 6  12.250.31.10 (12.250.31.10)  23.000 ms *  22.000 ms
 7  8.8.8.8 (8.8.8.8)  23.000 ms  23.000 ms  22.000 ms
```

#### --asn
```
$ traceroute --asn AS3320 8.8.8.8
traceroute to 8.8.8.8 (8.8.8.8) from AS3320, 30 hops max, 32 byte packets
traceroute to 8.8.8.8 (8.8.8.8) from probe ID 15072241 net "Deutsche Telekom AG" (DE), 30 hops max, 32 byte packets
 1  192.168.2.1 (192.168.2.1)  0.000 ms  0.000 ms  0.000 ms
 2  87.186.224.134 (87.186.224.134) * * *
 3  87.186.196.234 (87.186.196.234)  23.000 ms  23.000 ms  22.000 ms
 4  217.239.41.126 (217.239.41.126)  19.000 ms  20.000 ms  20.000 ms
 5  74.125.50.149 (74.125.50.149)  35.000 ms  35.000 ms  35.000 ms
 6  209.85.253.113 (209.85.253.113)  20.000 ms  24.000 ms  20.000 ms
 7  8.8.8.8 (8.8.8.8)  20.000 ms  20.000 ms  20.000 ms
traceroute to 8.8.8.8 (8.8.8.8) from probe ID 17089062 net "Deutsche Telekom AG" (DE), 30 hops max, 32 byte packets
 1  192.168.178.1 (192.168.178.1)  0.000 ms  0.000 ms  0.000 ms
 2  87.186.224.65 (87.186.224.65) * * *
 3  87.190.171.2 (87.190.171.2)  38.000 ms  5.000 ms  5.000 ms
 4  217.239.41.102 (217.239.41.102)  5.000 ms  6.000 ms  5.000 ms
 5  74.125.50.149 (74.125.50.149)  19.000 ms  19.000 ms  20.000 ms
 6  209.85.254.49 (209.85.254.49)  5.000 ms  5.000 ms  5.000 ms
 7  8.8.8.8 (8.8.8.8)  5.000 ms  5.000 ms  5.000 ms
traceroute to 8.8.8.8 (8.8.8.8) from probe ID 16057189 net "Deutsche Telekom AG" (DE), 30 hops max, 32 byte packets
 1  192.168.1.1 (192.168.1.1)  1.000 ms  1.000 ms  1.000 ms
 2  217.89.104.249 (217.89.104.249)  2.000 ms  2.000 ms  2.000 ms
...
...
...
 2  217.0.119.139 (217.0.119.139) * * *
 3  87.186.193.206 (87.186.193.206)  19.000 ms  20.000 ms  19.000 ms
 4  217.239.50.73 (217.239.50.73)  24.000 ms  58.000 ms  19.000 ms
 5  80.150.170.50 (80.150.170.50) * * *
 6  216.239.48.252 (216.239.48.252)  24.000 ms  23.000 ms  23.000 ms
 7  8.8.8.8 (8.8.8.8)  26.000 ms  23.000 ms  29.000 ms
traceroute to 8.8.8.8 (8.8.8.8) from probe ID 17099743 net "Deutsche Telekom AG" (DE), 30 hops max, 32 byte packets
 1  192.168.2.1 (192.168.2.1)  0.000 ms  0.000 ms  0.000 ms
 2  87.186.225.85 (87.186.225.85) * * *
 3  87.190.184.206 (87.190.184.206)  17.000 ms  16.000 ms  17.000 ms
 4  217.239.50.70 (217.239.50.70)  18.000 ms  18.000 ms  17.000 ms
 5  80.150.170.50 (80.150.170.50) * * *
 6  216.239.48.246 (216.239.48.246)  22.000 ms  21.000 ms  21.000 ms
 7  8.8.8.8 (8.8.8.8)  22.000 ms  22.000 ms  22.000 ms
```
