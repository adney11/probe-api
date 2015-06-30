@echo off

set ver=new
if not "%1" == "" set ver=%~1

echo Preparing build %ver%...

echo =======================================
echo Copy executables...

copy /Y dping.exe "dping-%ver%.exe"
copy /Y dtracert.exe "dtracert-%ver%.exe"

echo =======================================
echo Pack executables...

upx "dping-%ver%.exe"
upx "dtracert-%ver%.exe"

echo =======================================

pause
