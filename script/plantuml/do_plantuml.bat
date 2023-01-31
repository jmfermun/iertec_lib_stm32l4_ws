@echo off

cd /d %~dp0

call ..\global.bat

set PATH=%PATH_MSYS%;%PATH%

java -jar %PATH_PLANTUML%\plantuml.jar "../../docs/diagram" -o "../../docs/images"

pause
