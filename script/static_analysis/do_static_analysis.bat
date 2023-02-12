@echo off

cd /d %~dp0

call ..\global.bat

set PATH=%PATH_MSYS%;%PATH%
set PATH=%PATH_TARGET_COMPILER%;%PATH%
set BUILD_PATH=.\..\..\%PROJECT_NAME%\Release
set OUT_PATH=.\..\output\static_analysis

rem Generate the compilation database
call ../compile_db/do_compile_db.bat

cd /d %~dp0

rem Clang-tidy analysis
rem Note: Update compiler include paths "--extra-arg=-isystem" from "script\output\co-arm-none-eabi-gcc.lnt".
set CLANG_TIDY_SRC_PATTERN=^
 "%BUILD_PATH%\..\lib\iertec_lib_cc26xx\itf\*"^
 "%BUILD_PATH%\..\lib\iertec_lib_cc26xx\util\*"^
 "%BUILD_PATH%\..\lib\iertec_lib_cc26xx\fsm\*"^
 "%BUILD_PATH%\..\lib\iertec_lib_cc26xx\rtc\*"^
 "%BUILD_PATH%\..\lib\iertec_lib_cc26xx\task\*"^
 "%BUILD_PATH%\..\src\*"
set CLANG_TIDY_ARG=^
--checks=-*,bugprone-*,cert-*,clang-analyzer-*,misc-*,performance-*,portability-*,readability-*,^
-bugprone-easily-swappable-parameters,^
-readability-uppercase-literal-suffix,^
-readability-identifier-length,^
-readability-function-cognitive-complexity ^
--extra-arg=-isystem"c:/st/stm32cubeide_1.11.0/stm32cubeide/plugins/com.st.stm32cube.ide.mcu.externaltools.gnu-tools-for-stm32.10.3-2021.10.win32_1.0.100.202210260954/tools/bin/../lib/gcc/arm-none-eabi/10.3.1/../../../../arm-none-eabi/include/c++/10.3.1" ^
--extra-arg=-isystem"c:/st/stm32cubeide_1.11.0/stm32cubeide/plugins/com.st.stm32cube.ide.mcu.externaltools.gnu-tools-for-stm32.10.3-2021.10.win32_1.0.100.202210260954/tools/bin/../lib/gcc/arm-none-eabi/10.3.1/../../../../arm-none-eabi/include/c++/10.3.1/arm-none-eabi" ^
--extra-arg=-isystem"c:/st/stm32cubeide_1.11.0/stm32cubeide/plugins/com.st.stm32cube.ide.mcu.externaltools.gnu-tools-for-stm32.10.3-2021.10.win32_1.0.100.202210260954/tools/bin/../lib/gcc/arm-none-eabi/10.3.1/../../../../arm-none-eabi/include/c++/10.3.1/backward" ^
--extra-arg=-isystem"c:/st/stm32cubeide_1.11.0/stm32cubeide/plugins/com.st.stm32cube.ide.mcu.externaltools.gnu-tools-for-stm32.10.3-2021.10.win32_1.0.100.202210260954/tools/bin/../lib/gcc/arm-none-eabi/10.3.1/include"
--extra-arg=-isystem"c:/st/stm32cubeide_1.11.0/stm32cubeide/plugins/com.st.stm32cube.ide.mcu.externaltools.gnu-tools-for-stm32.10.3-2021.10.win32_1.0.100.202210260954/tools/bin/../lib/gcc/arm-none-eabi/10.3.1/include-fixed"
--extra-arg=-isystem"c:/st/stm32cubeide_1.11.0/stm32cubeide/plugins/com.st.stm32cube.ide.mcu.externaltools.gnu-tools-for-stm32.10.3-2021.10.win32_1.0.100.202210260954/tools/bin/../lib/gcc/arm-none-eabi/10.3.1/../../../../arm-none-eabi/include"

call clang-tidy %CLANG_TIDY_ARG% -p=%BUILD_PATH% %CLANG_TIDY_SRC_PATTERN% > %OUT_PATH%\report_clang-tidy.txt

rem CppCheck analysis
rem Note: Update compiler include paths "-I" from "script\output\co-co-arm-none-eabi-gcc.lnt".
set CPPCHECK_ARG=^
--enable=all ^
--output-file=%OUT_PATH%\report_cppcheck.txt ^
--platform=%PATH_MSYS%\..\share\cppcheck\platforms\arm32-wchar_t4 ^
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

call cppcheck %CPPCHECK_ARG% --project=%BUILD_PATH%\compile_commands.json

rem PC-Lint analysis
set PCLINT_SRC_PATTERN=".*/../src/.*|.*/iertec_lib_cc26xx/.*"

call python %PATH_PCLINT_CONFIG%\pclp_config.py --compiler=gcc --compiler-bin=arm-none-eabi-gcc --compilation-db=%BUILD_PATH%\compile_commands.json --config-output-lnt-file=%OUT_PATH%\co-project.lnt --generate-project-config --source-pattern=%PCLINT_SRC_PATTERN%
call %PATH_PCLINT%\pclp64.exe %OUT_PATH%\co-arm-none-eabi-gcc.lnt pclint_arg.lnt %OUT_PATH%\co-project.lnt > %OUT_PATH%\report_pclint.txt

pause
