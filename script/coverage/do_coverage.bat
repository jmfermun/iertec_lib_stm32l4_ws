@echo off
setlocal ENABLEDELAYEDEXPANSION

cd /d %~dp0

call ..\global.bat

set PATH=%PATH_MSYS%;%PATH%
set PATH_OUT=%~dp0..\output\coverage
set FILES_IN=

cd ..\..\%PROJECT_NAME%

if not exist %PATH_OUT%\report_coverage mkdir %PATH_OUT%\report_coverage

for %%f in (%PATH_OUT%\*.json) do (
    set FILES_IN=!FILES_IN! --add-tracefile %%f
)

gcovr %FILES_IN% --html-details -o %PATH_OUT%\report_coverage\report_coverage.html --decisions

pause
