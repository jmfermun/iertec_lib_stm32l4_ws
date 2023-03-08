cd "$(dirname "$0")"

source ../global.sh

# Use the following command to create a new empty configuration file
# uncrustify --update-config-with-doc > uncrustify.cfg

uncrustify --replace --no-backup -l C -c uncrustify.cfg \
../../$PROJECT_NAME/lib/iertec_lib_stm32l4/fsm/*.h \
../../$PROJECT_NAME/lib/iertec_lib_stm32l4/fsm/*.c \
../../$PROJECT_NAME/lib/iertec_lib_stm32l4/itf/*.h \
../../$PROJECT_NAME/lib/iertec_lib_stm32l4/itf/*.c \
../../$PROJECT_NAME/lib/iertec_lib_stm32l4/rtc/*.h \
../../$PROJECT_NAME/lib/iertec_lib_stm32l4/rtc/*.c \
../../$PROJECT_NAME/lib/iertec_lib_stm32l4/rtos/*.h \
../../$PROJECT_NAME/lib/iertec_lib_stm32l4/rtos/*.c \
../../$PROJECT_NAME/lib/iertec_lib_stm32l4/task/*.h \
../../$PROJECT_NAME/lib/iertec_lib_stm32l4/task/*.c \
../../$PROJECT_NAME/lib/iertec_lib_stm32l4/util/*.h \
../../$PROJECT_NAME/lib/iertec_lib_stm32l4/util/*.c \
../../$PROJECT_NAME/src/*.h \
../../$PROJECT_NAME/src/*.c