@echo off

cd /d %~dp0

call ..\global.bat

set PATH=%PATH_MSYS%;%PATH%

for /f "delims=" %%i in ('python get_fw_version.py %PROJECT_NAME%') do set PROJECT_NUMBER=%%i
doxygen doxyfile

pause
