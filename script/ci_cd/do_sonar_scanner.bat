@echo off

set PATH=C:\Desarrollo\Programas\sonar-scanner-4.8.0.2856-windows\bin;%PATH%

cd /d %~dp0
cd ../../iertec_lib_stm32l4_test

sonar-scanner.bat -Dsonar.login=squ_553041b040e200cedad8028821938626c6467443 -Dproject.settings=%~dp0sonar-project.properties
