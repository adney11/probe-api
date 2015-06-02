@echo off
REM This batch file is creating folder openssl_include_prepared\openssl and fills it with correct C header files.

set outfold="%~dp0..\..\openssl_include_prepared\openssl"

pushd "%~dp0..\..\openssl"
if not exist %outfold% mkdir %outfold%
copy /Y *.h %outfold%
copy /Y ssl\*.h %outfold%
set cnt=0
for /r crypto %%x in (*.h) do call :onefile "%%x"
echo        %cnt% file(s) copied.
REM FOR /D %%x IN (crypto\*) DO copy /Y "%%x\*.h" %outfold%

popd

goto end

REM ==========================================================

:end

REM pause
goto :eof

REM ==========================================================
:onefile
echo %~1
copy /Y %1 %outfold% > nul
set /a cnt=cnt+1
goto :eof

REM ==========================================================
