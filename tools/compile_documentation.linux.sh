#!/bin/bash

PATH=$(dirname "$0")/../bin:$PATH
pushd "$(dirname "$0")"

echo Rebuilding documentation...
echo Note! You need the following unix utils available via PATH environment variable:
echo - gawk
echo 

# ======================================================
echo README.md...

gawk -f ./file-formatter.awk README.md.in > ../README.md

# ======================================================

echo doc/dping.linux.md...

gawk -f ./file-formatter.awk dping.md.in > ../doc/dping.linux.md

# ======================================================

echo doc/dtraceroute.linux.md...

gawk -f ./file-formatter.awk dtraceroute.md.in > ../doc/dtraceroute.linux.md

# ======================================================

echo Done!
popd

# ======================================================
