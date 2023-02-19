@echo off

cd /d %~dp0

call ..\global.bat

set PATH=%PATH_MSYS%;%PATH%
set CEEDLING_MAIN_PROJECT_FILE=%~dp0\project_unit_host.yml

cd ..\..\%PROJECT_NAME%

call ceedling.bat clobber logging test:all 2>&1 | tee ..\script\output\test\log_unit_host.txt

call %~dp0..\coverage\gcov_host.bat

cd %~dp0

pause
