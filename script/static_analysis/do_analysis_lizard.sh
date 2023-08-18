cd "$(dirname "$0")"

source ../global.sh

PATH_OUT=$PWD/../output/static_analysis

cd ../../$PROJECT_NAME

mkdir -p $PATH_OUT

lizard -Tcyclomatic_complexity=15 -Tparameter_count=7 -Tnloc=200 -Tmax_nested_structures=5 --modified --extension NS -o $PATH_OUT/log_lizard.html lib/iertec_lib_stm32l4/* src/*
