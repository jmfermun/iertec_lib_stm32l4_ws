cd "$(dirname "$0")"

source ../global.sh

export CEEDLING_MAIN_PROJECT_FILE=$PWD/project_unit_host.yml
PATH_LOG=$PWD/../output/test
FILE_LOG=$PATH_LOG/log_unit_host.txt
PATH_COVERAGE=$PWD/../coverage

cd ../../$PROJECT_NAME

mkdir -p $PATH_LOG
touch $FILE_LOG

ceedling clobber logging test:all 2>&1 | tee $FILE_LOG

bash $PATH_COVERAGE/gcov_host.sh

cd "$(dirname "$0")"
