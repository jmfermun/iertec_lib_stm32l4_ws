@echo off

call %~dp0..\global.bat

set PATH=%PATH_MSYS%;%PATH%
set PATH_OUT=%~dp0..\output\coverage\
set PATH_IN=%~dp0..\output\test\build_unit_host\test\out\c

if not exist %PATH_OUT% mkdir %PATH_OUT%

gcovr %PATH_IN% --json --gcov-executable=gcov.exe -o %PATH_OUT%\report_coverage_host.json --delete --decisions
