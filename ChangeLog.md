# ProbeAPI Tools History (ChangeLog)

## Next version

Changes:



## Version 1.3.28 (6 June 2015)

Changes:
* added --no-delays command line option
* use zlib for cURL in Windows
* Minor bugs fixed
* added cmake scripts for crossplatform building
* supported compilation environments:
  * native MSVC
  * cmake: auto-generated MSVC
  * cmake: MinGW for Windows + makefiles + gcc
  * cmake: Unix makefiles + gcc in Linux
* use submodules in git to get dependent sources
* redistribute pre-built RPM packet for linux


## Version 1.1.22 (22 May 2015)

* fixed a bug in dping which caused skipping of final statistics print
* documentation is generated automatically now
* other code improvements

**WARNING!** dtraceroute's --asn mode is broken in this release!

 
## Version 1.1.20 (21 May 2015)

* dtraceroute project is implemented
* program termination is now more correct
* source code was deduplicated


## Version 1.0.16 (18 May 2015)

* Added license, copyrights, documentation.
* Disabled default mode which caused pinging from country with most of endpoints online.
* Smaller curl runtime (excluded unused protocols).


## Version 1.0.13 (13 May 2015)

All dping utility features from wiki are implemented:

* --list-country - List available countries.
* --list-asn {code} - List available ASNs for specified country code (ISO 3166-1 alpha-2).
* --country {code} - Ping from specified 2 letter country code (ISO 3166-1 alpha-2).
* --asn {id} - Ping from specified ASN.

Usage examples:
* dping --asn AS12357 8.8.8.8
* dping --country US 8.8.8.8


## Version 0.5.7 (11 May 2015)

dping utility got its first few features implemented.

It currently allows two main command line modes:

* --list-country - List available countries.
* --country {code} - Ping from specified 2 letter country code (ISO 3166-1 alpha-2).
