@echo off

call %~dp0..\global.bat

set PATH=%PATH_MSYS%;%PATH%
set PATH=%PATH_TARGET_COMPILER%;%PATH%
set PATH_OUT=%~dp0..\output\coverage

if not exist %PATH_OUT% mkdir %PATH_OUT%

gcovr %1 --json --gcov-executable=arm-none-eabi-gcov.exe -o %PATH_OUT%\report_coverage_%2.json --delete --decisions
