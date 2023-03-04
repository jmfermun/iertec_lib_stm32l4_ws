cd "$(dirname "$0")"

source ../global.sh

PATH=$PATH_TARGET_COMPILER:$PATH
PATH_OUT=$PWD/../output/coverage

cd ../../$PROJECT_NAME

mkdir -p $PATH_OUT

gcovr $1 --json --gcov-executable=arm-none-eabi-gcov.exe -o $PATH_OUT/report_coverage_$2.json --delete --decisions
