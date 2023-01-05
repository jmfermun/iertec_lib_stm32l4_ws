@echo off

cd /d %~dp0

call ..\global.bat

set PATH=%PATH_MSYS%;%PATH%

cd ..\..\%PROJECT_NAME%\Release

compiledb --verbose --parse ..\..\.metadata\.plugins\org.eclipse.cdt.ui\%PROJECT_NAME%.build.log

pause
