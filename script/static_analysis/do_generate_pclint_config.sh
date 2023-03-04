cd "$(dirname "$0")"

source ../global.sh

PATH=$PATH_TARGET_COMPILER:$PATH
PATH_OUT=../output/static_analysis

# Compile imposter program
gcc -Os $PATH_PCLINT_CONFIG/imposter.c -o $PATH_PCLINT_CONFIG/imposter

mkdir -p $PATH_OUT

# Generate PC-lint Plus compiler configuration
python $PATH_PCLINT_CONFIG/pclp_config.py --compiler=gcc --compiler-bin=arm-none-eabi-gcc --config-output-lnt-file=$PATH_OUT/co-arm-none-eabi-gcc.lnt --config-output-header-file=$PATH_OUT/co-arm-none-eabi-gcc.h --generate-compiler-config
