#!/bin/bash

PATH=$(dirname "$0")/../bin:$PATH
pushd "$(dirname "$0")"

echo Rebuilding documentation...
echo Note! You need the following unix utils available via PATH variable:
echo - awk
echo 

# ======================================================
echo README.md...

awk -f ./file-formatter.awk README.md.in > ../README2.md

# ======================================================

echo doc/dping.linux.md...

awk -f ./file-formatter.awk dping.md.in > ../doc/dping.linux.md

# ======================================================

echo doc/dtraceroute.linux.md...

awk -f ./file-formatter.awk dtraceroute.md.in > ../doc/dtraceroute.linux.md

# ======================================================

echo Done!
popd

# ======================================================
