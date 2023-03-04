cd "$(dirname "$0")"

source ../global.sh

PATH=$PWD:$PATH
PATH=$PATH_TARGET_COMPILER:$PATH
PATH=$PATH_OPENOCD:$PATH
export CEEDLING_MAIN_PROJECT_FILE=$PWD/project_unit_target.yml
PATH_LOG=../script/output/test
FILE_LOG=$PATH_LOG/log_unit_target.txt
export PATH_COVERAGE=$PWD/../coverage
export PATH_OPENOCD_S=$PATH_OPENOCD_S

cd ../../$PROJECT_NAME

mkdir -p $PATH_LOG
touch $FILE_LOG

ceedling clobber logging test:all 2>&1 | tee $LOG_FILE

cd "$(dirname "$0")"
