@echo off

cd /d %~dp0

call ..\global.bat

set PATH=%~dp0;%PATH%
set PATH=%PATH_MSYS%;%PATH%
set PATH=%PATH_TARGET_COMPILER%;%PATH%
set PATH=%PATH_OPENOCD%;%PATH%
set CEEDLING_MAIN_PROJECT_FILE=%~dp0\project_integration_target.yml

cd ..\..\%PROJECT_NAME%

call ceedling.bat clobber logging test:all 2>&1 | tee ..\script\output\test\log_integration_target.txt

cd %~dp0

pause
