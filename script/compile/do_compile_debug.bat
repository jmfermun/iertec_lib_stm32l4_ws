@echo off

cd /d %~dp0

call ..\global.bat

set PATH=%PATH_MSYS%;%PATH%
set PATH=%PATH_IDE%;%PATH%
set PATH_OUT=script\output\compile

cd ..\..\

if not exist %PATH_OUT% mkdir %PATH_OUT%

stm32cubeidec --launcher.suppressErrors -nosplash -application org.eclipse.cdt.managedbuilder.core.headlessbuild -data %cd% -cleanBuild %PROJECT_NAME%/Debug | tee %PATH_OUT%\compile_debug_log.txt

cd %PROJECT_NAME%\Debug

compiledb --verbose --parse ..\..\%PATH_OUT%\compile_debug_log.txt

pause
