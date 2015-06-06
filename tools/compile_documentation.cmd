@echo off

set path=%~dp0../Release;%PATH%
pushd "%~dp0"

echo Rebuilding documentation...
echo Note! You need the following unix utils available via PATH variable:
echo - gawk
REM pause
echo.

REM ======================================================
echo README.md...

gawk -f ./file-formatter.awk README.md.in > ../README.md

REM ======================================================

echo doc/dping.windows.md...

gawk -f ./file-formatter.awk dping.md.in > ../doc/dping.windows.md

REM ======================================================

echo doc/dtraceroute.windows.md...

gawk -f ./file-formatter.awk dtraceroute.md.in > ../doc/dtraceroute.windows.md

REM ======================================================

echo Done!
popd
pause

goto :eof

REM ======================================================
