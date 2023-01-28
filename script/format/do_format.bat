@echo off

cd /d %~dp0

call ..\global.bat

set PATH=%PATH_MSYS%;%PATH%

rem Use the following command to create a new empty configuration file
rem uncrustify --update-config-with-doc > uncrustify.cfg

uncrustify --replace --no-backup -l C -c uncrustify.cfg ^
../../%PROJECT_NAME%/lib/iertec_lib_stm32l4/itf/*.h ^
../../%PROJECT_NAME%/lib/iertec_lib_stm32l4/itf/*.c ^
../../%PROJECT_NAME%/lib/iertec_lib_stm32l4/rtos/*.h ^
../../%PROJECT_NAME%/lib/iertec_lib_stm32l4/rtos/*.c ^
../../%PROJECT_NAME%/lib/iertec_lib_stm32l4/util/*.h ^
../../%PROJECT_NAME%/lib/iertec_lib_stm32l4/util/*.c ^
../../%PROJECT_NAME%/src/*.h ^
../../%PROJECT_NAME%/src/*.c

pause
