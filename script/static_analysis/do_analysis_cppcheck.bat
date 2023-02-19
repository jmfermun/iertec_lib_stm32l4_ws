@echo off

cd /d %~dp0

call ..\global.bat

set PATH=%PATH_MSYS%;%PATH%
set PATH=%PATH_TARGET_COMPILER%;%PATH%
set BUILD_PATH=.\..\..\%PROJECT_NAME%\Release
set OUT_PATH=.\..\output\static_analysis

rem CppCheck analysis
rem Note: Update compiler include paths "-I" from "script\output\co-co-arm-none-eabi-gcc.lnt".
set CPPCHECK_ARG=^
--enable=all ^
--output-file=%OUT_PATH%\report_cppcheck.txt ^
--platform=%PATH_CPPCHECK_SHARE%\platforms\arm32-wchar_t4 ^
-I"c:/st/stm32cubeide_1.11.0/stm32cubeide/plugins/com.st.stm32cube.ide.mcu.externaltools.gnu-tools-for-stm32.10.3-2021.10.win32_1.0.100.202210260954/tools/bin/../lib/gcc/arm-none-eabi/10.3.1/../../../../arm-none-eabi/include/c++/10.3.1" ^
-I"c:/st/stm32cubeide_1.11.0/stm32cubeide/plugins/com.st.stm32cube.ide.mcu.externaltools.gnu-tools-for-stm32.10.3-2021.10.win32_1.0.100.202210260954/tools/bin/../lib/gcc/arm-none-eabi/10.3.1/../../../../arm-none-eabi/include/c++/10.3.1/arm-none-eabi" ^
-I"c:/st/stm32cubeide_1.11.0/stm32cubeide/plugins/com.st.stm32cube.ide.mcu.externaltools.gnu-tools-for-stm32.10.3-2021.10.win32_1.0.100.202210260954/tools/bin/../lib/gcc/arm-none-eabi/10.3.1/../../../../arm-none-eabi/include/c++/10.3.1/backward" ^
-I"c:/st/stm32cubeide_1.11.0/stm32cubeide/plugins/com.st.stm32cube.ide.mcu.externaltools.gnu-tools-for-stm32.10.3-2021.10.win32_1.0.100.202210260954/tools/bin/../lib/gcc/arm-none-eabi/10.3.1/include" ^
-I"c:/st/stm32cubeide_1.11.0/stm32cubeide/plugins/com.st.stm32cube.ide.mcu.externaltools.gnu-tools-for-stm32.10.3-2021.10.win32_1.0.100.202210260954/tools/bin/../lib/gcc/arm-none-eabi/10.3.1/include-fixed" ^
-I"c:/st/stm32cubeide_1.11.0/stm32cubeide/plugins/com.st.stm32cube.ide.mcu.externaltools.gnu-tools-for-stm32.10.3-2021.10.win32_1.0.100.202210260954/tools/bin/../lib/gcc/arm-none-eabi/10.3.1/../../../../arm-none-eabi/include" ^
--include=%OUT_PATH%\co-arm-none-eabi-gcc.h ^
--suppress=missingIncludeSystem ^
--suppress=unusedFunction ^
--suppress=*:*/Core/* ^
--suppress=*:*/Drivers/* ^
--suppress=*:*/Middlewares/*

cppcheck %CPPCHECK_ARG% --project=%BUILD_PATH%\compile_commands.json

pause
