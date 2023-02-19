@echo off

cd /d %~dp0

call ..\global.bat

set PATH=%PATH_MSYS%;%PATH%
set PATH=%PATH_TARGET_COMPILER%;%PATH%
set BUILD_PATH=.\..\..\%PROJECT_NAME%\Release
set OUT_PATH=.\..\output\static_analysis

rem Clang-tidy analysis
rem Note: Update compiler include paths "--extra-arg=-isystem" from "script\output\co-arm-none-eabi-gcc.lnt".
set CLANG_TIDY_SRC_PATTERN=^
 "%BUILD_PATH%\..\lib\iertec_lib_stm32l4\fsm\*.h"^
 "%BUILD_PATH%\..\lib\iertec_lib_stm32l4\fsm\*.c"^
 "%BUILD_PATH%\..\lib\iertec_lib_stm32l4\itf\*.h"^
 "%BUILD_PATH%\..\lib\iertec_lib_stm32l4\itf\*.c"^
 "%BUILD_PATH%\..\lib\iertec_lib_stm32l4\rtc\*.h"^
 "%BUILD_PATH%\..\lib\iertec_lib_stm32l4\rtc\*.c"^
 "%BUILD_PATH%\..\lib\iertec_lib_stm32l4\rtos\*.h"^
 "%BUILD_PATH%\..\lib\iertec_lib_stm32l4\rtos\*.c"^
 "%BUILD_PATH%\..\lib\iertec_lib_stm32l4\task\*.h"^
 "%BUILD_PATH%\..\lib\iertec_lib_stm32l4\task\*.c"^
 "%BUILD_PATH%\..\lib\iertec_lib_stm32l4\util\*.h"^
 "%BUILD_PATH%\..\lib\iertec_lib_stm32l4\util\*.c"^
 "%BUILD_PATH%\..\src\*.h"
 "%BUILD_PATH%\..\src\*.c"
set CLANG_TIDY_ARG=^
--checks=-*,bugprone-*,cert-*,clang-analyzer-*,misc-*,performance-*,portability-*,readability-*,^
-bugprone-easily-swappable-parameters,^
-readability-uppercase-literal-suffix,^
-readability-identifier-length,^
-readability-function-cognitive-complexity ^
--extra-arg=-isystem"c:/st/stm32cubeide_1.11.0/stm32cubeide/plugins/com.st.stm32cube.ide.mcu.externaltools.gnu-tools-for-stm32.10.3-2021.10.win32_1.0.100.202210260954/tools/bin/../lib/gcc/arm-none-eabi/10.3.1/../../../../arm-none-eabi/include/c++/10.3.1" ^
--extra-arg=-isystem"c:/st/stm32cubeide_1.11.0/stm32cubeide/plugins/com.st.stm32cube.ide.mcu.externaltools.gnu-tools-for-stm32.10.3-2021.10.win32_1.0.100.202210260954/tools/bin/../lib/gcc/arm-none-eabi/10.3.1/../../../../arm-none-eabi/include/c++/10.3.1/arm-none-eabi" ^
--extra-arg=-isystem"c:/st/stm32cubeide_1.11.0/stm32cubeide/plugins/com.st.stm32cube.ide.mcu.externaltools.gnu-tools-for-stm32.10.3-2021.10.win32_1.0.100.202210260954/tools/bin/../lib/gcc/arm-none-eabi/10.3.1/../../../../arm-none-eabi/include/c++/10.3.1/backward" ^
--extra-arg=-isystem"c:/st/stm32cubeide_1.11.0/stm32cubeide/plugins/com.st.stm32cube.ide.mcu.externaltools.gnu-tools-for-stm32.10.3-2021.10.win32_1.0.100.202210260954/tools/bin/../lib/gcc/arm-none-eabi/10.3.1/include" ^
--extra-arg=-isystem"c:/st/stm32cubeide_1.11.0/stm32cubeide/plugins/com.st.stm32cube.ide.mcu.externaltools.gnu-tools-for-stm32.10.3-2021.10.win32_1.0.100.202210260954/tools/bin/../lib/gcc/arm-none-eabi/10.3.1/include-fixed" ^
--extra-arg=-isystem"c:/st/stm32cubeide_1.11.0/stm32cubeide/plugins/com.st.stm32cube.ide.mcu.externaltools.gnu-tools-for-stm32.10.3-2021.10.win32_1.0.100.202210260954/tools/bin/../lib/gcc/arm-none-eabi/10.3.1/../../../../arm-none-eabi/include"

clang-tidy %CLANG_TIDY_ARG% -p=%BUILD_PATH% %CLANG_TIDY_SRC_PATTERN% > %OUT_PATH%\report_clang-tidy.txt

pause
