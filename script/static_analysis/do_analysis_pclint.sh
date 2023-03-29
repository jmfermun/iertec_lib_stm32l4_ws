cd "$(dirname "$0")"

source ../global.sh

PATH=$PATH_TARGET_COMPILER:$PATH
PATH_BUILD=../../$PROJECT_NAME/Release
PATH_OUT=../output/static_analysis

# Compiler selection
PCLINT_COMPILER=gcc
PCLINT_COMPILER_BIN=arm-none-eabi-gcc

# PC-Lint analysis
PCLINT_SRC_PATTERN=".*/../src/.*|.*/iertec_lib_stm32l4/.*"

python $PATH_PCLINT_CONFIG/pclp_config.py --compiler=$PCLINT_COMPILER --compiler-bin=$PCLINT_COMPILER_BIN --compilation-db=$PATH_BUILD/compile_commands.json --config-output-lnt-file=$PATH_OUT/co-project.lnt --generate-project-config --source-pattern=$PCLINT_SRC_PATTERN
$PATH_PCLINT/pclp64.exe msg_sonarqube.lnt $PATH_OUT/co-$PCLINT_COMPILER_BIN.lnt pclint_arg.lnt $PATH_OUT/co-project.lnt > $PATH_OUT/report_pclint.xml
