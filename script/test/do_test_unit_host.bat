@echo off

cd /d %~dp0

call ..\global.bat

set PATH=%PATH_MSYS%;%PATH%
set CEEDLING_MAIN_PROJECT_FILE=%~dp0\project_unit_host.yml
set LOG_DIR=..\script\output\test
set LOG_FILE=%LOG_DIR%\log_unit_host.txt

cd ..\..\%PROJECT_NAME%

if not exist %LOG_DIR% mkdir %LOG_DIR%
if not exist %LOG_FILE% type nul > %LOG_FILE%

ceedling.bat clobber logging test:all 2>&1 | tee %LOG_FILE%

%~dp0..\coverage\gcov_host.bat

cd %~dp0

pause
