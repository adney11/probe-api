@echo off

set parts="%~dp0text_parts"
set dest="%~dp0.."
set bin="%~dp0..\Release"

set strip_first_line=awk "{ a[i++] = $0 } END { for (j=1; j<i; j++) print a[j] }"
set strip_first_two_lines=awk "{ a[i++] = $0 } END { for (j=2; j<i; j++) print a[j] }"

echo Rebuilding documentation...
echo Note! You need the following unix utils available via PATH variable:
echo * awk
REM pause
echo.

REM ======================================================
echo README.md...
set destfile=%dest%\README.md

copy %parts%\README_10.md %destfile%
%bin%\dping.exe --version | %strip_first_line% >> %destfile%
type %parts%\README_100.md >> %destfile%

REM ======================================================
echo dping.md...
set tool_name=dping
set destfile=%dest%\%tool_name%.md
set tool=%bin%\%tool_name%.exe
set part_pref=%parts%\%tool_name%

copy %part_pref%_10.md %destfile%
%tool% --help | %strip_first_two_lines% >> %destfile%
type %part_pref%_20.md >> %destfile%
call :print_with_middle_trim "--list-country" 10 2 1
type %part_pref%_30.md >> %destfile%
call :print_with_middle_trim "--list-asn ES" 10 2 1
type %part_pref%_40.md >> %destfile%
call :print_with_middle_trim "--country US 8.8.8.8" 10 7 1
type %part_pref%_50.md >> %destfile%
call :print_with_middle_trim "--asn AS3352 8.8.8.8" 10 7 1
type %part_pref%_60.md >> %destfile%

REM ======================================================
echo dtraceroute.md...
set tool_name=dtracert
set destfile=%dest%\dtraceroute.md
set tool=%bin%\%tool_name%.exe
set part_pref=%parts%\dtraceroute

copy %part_pref%_10.md %destfile%
%tool% --help | %strip_first_two_lines% >> %destfile%
type %part_pref%_20.md >> %destfile%
call :print_with_middle_trim "--list-country" 10 2 1
type %part_pref%_30.md >> %destfile%
call :print_with_middle_trim "--list-asn ES" 10 2 1
type %part_pref%_40.md >> %destfile%
call :print_with_middle_trim "--country US 8.8.8.8" 20 14 3
type %part_pref%_50.md >> %destfile%
call :print_with_middle_trim "--asn AS3320 8.8.8.8" 20 14 3
type %part_pref%_60.md >> %destfile%

REM ======================================================

echo Done!
pause

goto :eof

REM ======================================================
:print_with_middle_trim
REM usage:
REM call :print_with_middle_trim "arguments for tool" [keep_first_N] [keep_last_N] [insert_N]
set args=%~1
set first=%~2
set last=%~3
set insert=%~4

echo %tool_name% %args%...
echo $ %tool_name% %args%>> %destfile%
%tool% %args% | awk "{ a[i++] = $0 } END { b=1; for (j=0; j<i; j++) if(j < %first% || j >= i-%last%) print a[j]; else if(b>0) { for(k=0; k<%insert%; k++) print \"...\"; b=0; } }" >> %destfile%
goto :eof

REM ======================================================
