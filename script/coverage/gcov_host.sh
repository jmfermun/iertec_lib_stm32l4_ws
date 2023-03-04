cd "$(dirname "$0")"

source ../global.sh

PATH_OUT=$PWD/../output/coverage/
PATH_IN=$PWD/../output/test/build_unit_host/test/out/c

cd ../../$PROJECT_NAME

mkdir -p $PATH_OUT

gcovr $PATH_IN --json --gcov-executable=gcov.exe -o $PATH_OUT/report_coverage_host.json --delete --decisions
