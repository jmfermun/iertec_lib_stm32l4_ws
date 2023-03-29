cd "$(dirname "$0")"

source ../global.sh

PATH=$PATH_TARGET_COMPILER:$PATH
PATH_OUT=../output/static_analysis

# Compiler selection
PCLINT_COMPILER=gcc
PCLINT_COMPILER_BIN=arm-none-eabi-gcc

# Compile imposter program
gcc -Os $PATH_PCLINT_CONFIG/imposter.c -o $PATH_PCLINT_CONFIG/imposter

mkdir -p $PATH_OUT

# Generate PC-lint Plus compiler configuration
python $PATH_PCLINT_CONFIG/pclp_config.py --compiler=$PCLINT_COMPILER --compiler-bin=$PCLINT_COMPILER_BIN --config-output-lnt-file=$PATH_OUT/co-$PCLINT_COMPILER_BIN.lnt --config-output-header-file=$PATH_OUT/co-$PCLINT_COMPILER_BIN.h --generate-compiler-config
