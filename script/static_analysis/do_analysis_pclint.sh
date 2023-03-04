cd "$(dirname "$0")"

source ../global.sh

PATH=$PATH_TARGET_COMPILER:$PATH
PATH_BUILD=../../$PROJECT_NAME/Release
PATH_OUT=../output/static_analysis

# PC-Lint analysis
PCLINT_SRC_PATTERN=".*/../src/.*|.*/iertec_lib_stm32l4/.*"

python $PATH_PCLINT_CONFIG/pclp_config.py --compiler=gcc --compiler-bin=arm-none-eabi-gcc --compilation-db=$PATH_BUILD/compile_commands.json --config-output-lnt-file=$PATH_OUT/co-project.lnt --generate-project-config --source-pattern=$PCLINT_SRC_PATTERN
$PATH_PCLINT/pclp64.exe $PATH_OUT/co-arm-none-eabi-gcc.lnt pclint_arg.lnt $PATH_OUT/co-project.lnt > $PATH_OUT/report_pclint.txt
