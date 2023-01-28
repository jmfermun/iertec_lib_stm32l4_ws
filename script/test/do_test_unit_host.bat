@echo off

cd /d %~dp0

call ..\global.bat

set PATH=%PATH_MSYS%;%PATH%
set CEEDLING_MAIN_PROJECT_FILE=%~dp0\project_unit_host.yml

cd ..\..\%PROJECT_NAME%

call ceedling.bat clobber logging test:all

cd %~dp0

pause
