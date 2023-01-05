@echo off

cd /d %~dp0

call ..\global.bat

set PATH=%PATH_MSYS%;%PATH%
set PATH=%PATH_TARGET_COMPILER%;%PATH%
set PATH_OUT=%~dp0..\output\coverage

cd ..\..\%PROJECT_NAME%

if not exist %PATH_OUT%\report_coverage mkdir %PATH_OUT%\report_coverage

gcovr --add-tracefile %PATH_OUT%\*.json --html-details -o %PATH_OUT%\report_coverage\report_coverage.html --decisions

pause
