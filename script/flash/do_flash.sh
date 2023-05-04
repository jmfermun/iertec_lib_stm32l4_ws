cd "$(dirname "$0")"

source ../global.sh

PATH=$PATH_TARGET_PROGRAMMER:$PATH

STM32_Programmer_CLI --connect port=SWD --erase all --download bin/$PROJECT_NAME.hex --verify -rst
