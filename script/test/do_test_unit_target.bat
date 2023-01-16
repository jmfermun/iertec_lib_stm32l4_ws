@echo off

cd /d %~dp0

call ..\global.bat

set PATH=%~dp0;%PATH%
set PATH=%PATH_MSYS%;%PATH%
set PATH=%PATH_TARGET_COMPILER%;%PATH%
set PATH=%PATH_OPENOCD%;%PATH%
set CEEDLING_MAIN_PROJECT_FILE=%~dp0\project_unit_target.yml

cd ..\..\%PROJECT_NAME%

call ceedling.bat clobber logging test:all

cd %~dp0

pause
