@echo off

cd /d %~dp0

call ..\global.bat

set PATH=%PATH_IDE%;%PATH%

cd ..\..\

stm32cubeidec --launcher.suppressErrors -nosplash -application org.eclipse.cdt.managedbuilder.core.headlessbuild -data %cd% -cleanBuild %PROJECT_NAME%/Debug

pause
