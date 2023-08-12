cd "$(dirname "$0")"

source ../global.sh

PATH=$PATH_TARGET_COMPILER:$PATH

BUILD_PATH=../../$PROJECT_NAME/Release
PATH_OUT=$PWD/../output/static_analysis
PATH_HTML=$PATH_OUT/report_clang_tidy

# Clang-tidy analysis
# Note: Update compiler include paths "--extra-arg=-isystem" from "script/output/co-arm-none-eabi-gcc.lnt".
CLANG_TIDY_SRC_PATTERN='
'$BUILD_PATH'/../lib/iertec_lib_stm32l4/*/*.h
'$BUILD_PATH'/../lib/iertec_lib_stm32l4/*/*.c
'$BUILD_PATH'/../src/*.h
'$BUILD_PATH'/../src/*.c
'
CLANG_TIDY_ARG='
--checks=-*,bugprone-*,cert-*,clang-analyzer-*,misc-*,performance-*,portability-*,readability-*,-bugprone-easily-swappable-parameters,-readability-uppercase-literal-suffix,-readability-identifier-length,-readability-function-cognitive-complexity
--extra-arg=-isystem"/c/st/stm32cubeide_1.11.0/stm32cubeide/plugins/com.st.stm32cube.ide.mcu.externaltools.gnu-tools-for-stm32.10.3-2021.10.win32_1.0.100.202210260954/tools/bin/../lib/gcc/arm-none-eabi/10.3.1/../../../../arm-none-eabi/include/c++/10.3.1/"
--extra-arg=-isystem"/c/st/stm32cubeide_1.11.0/stm32cubeide/plugins/com.st.stm32cube.ide.mcu.externaltools.gnu-tools-for-stm32.10.3-2021.10.win32_1.0.100.202210260954/tools/bin/../lib/gcc/arm-none-eabi/10.3.1/../../../../arm-none-eabi/include/c++/10.3.1/arm-none-eabi"
--extra-arg=-isystem"/c/st/stm32cubeide_1.11.0/stm32cubeide/plugins/com.st.stm32cube.ide.mcu.externaltools.gnu-tools-for-stm32.10.3-2021.10.win32_1.0.100.202210260954/tools/bin/../lib/gcc/arm-none-eabi/10.3.1/../../../../arm-none-eabi/include/c++/10.3.1/backward"
--extra-arg=-isystem"/c/st/stm32cubeide_1.11.0/stm32cubeide/plugins/com.st.stm32cube.ide.mcu.externaltools.gnu-tools-for-stm32.10.3-2021.10.win32_1.0.100.202210260954/tools/bin/../lib/gcc/arm-none-eabi/10.3.1/include"
--extra-arg=-isystem"/c/st/stm32cubeide_1.11.0/stm32cubeide/plugins/com.st.stm32cube.ide.mcu.externaltools.gnu-tools-for-stm32.10.3-2021.10.win32_1.0.100.202210260954/tools/bin/../lib/gcc/arm-none-eabi/10.3.1/include-fixed"
--extra-arg=-isystem"/c/st/stm32cubeide_1.11.0/stm32cubeide/plugins/com.st.stm32cube.ide.mcu.externaltools.gnu-tools-for-stm32.10.3-2021.10.win32_1.0.100.202210260954/tools/bin/../lib/gcc/arm-none-eabi/10.3.1/../../../../arm-none-eabi/include"
'

mkdir -p $PATH_OUT
mkdir -p $PATH_HTML

clang-tidy $CLANG_TIDY_ARG -p=$BUILD_PATH $CLANG_TIDY_SRC_PATTERN > $PATH_OUT/report_clang_tidy.txt || true
clang-tidy-html $PATH_OUT/report_clang_tidy.txt -o $PATH_HTML/report_clang_tidy.html
