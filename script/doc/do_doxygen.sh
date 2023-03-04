cd "$(dirname "$0")"

source ../global.sh

export PROJECT_NUMBER=$(python get_fw_version.py $PROJECT_NAME)

doxygen doxyfile
