cd "$(dirname "$0")"

source ../global.sh

# Use the following command to create a default configuration file
# uncrustify --update-config-with-doc > uncrustify_new.cfg

# Use the following command to update the configuration file for a new uncrustify version
# uncrustify --update-config-with-doc -c uncrustify.cfg > uncrustify_new.cfg

uncrustify --replace --no-backup -l C -c uncrustify.cfg \
../../$PROJECT_NAME/lib/iertec_lib_stm32l4/crypt/*.h \
../../$PROJECT_NAME/lib/iertec_lib_stm32l4/crypt/*.c \
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
