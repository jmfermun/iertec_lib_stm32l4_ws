cd "$(dirname "$0")"

source ../global.sh

cd ../../$PROJECT_NAME

PATH_OUT=../script/output/coverage
FILES_IN=""

for filename in $PATH_OUT/*.json; do
    FILES_IN="$FILES_IN --add-tracefile $filename"
done

mkdir -p $PATH_OUT/report_coverage

gcovr $FILES_IN --html-details -o $PATH_OUT/report_coverage/report_coverage.html --decisions
gcovr $FILES_IN --xml -o $PATH_OUT/report_coverage/report_coverage.xml --decisions