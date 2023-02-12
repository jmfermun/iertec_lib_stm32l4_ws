@echo off

cd /d %~dp0

call ..\global.bat

set PATH=%PATH_MSYS%;%PATH%
set PATH=%PATH_TARGET_COMPILER%;%PATH%
set OUT_PATH=.\..\output\static_analysis

rem Compile imposter program
gcc -Os %PATH_PCLINT_CONFIG%\imposter.c -o %PATH_PCLINT_CONFIG%\imposter

if not exist "%OUT_PATH%" mkdir "%OUT_PATH%"

rem Generate PC-lint Plus compiler configuration
python %PATH_PCLINT_CONFIG%\pclp_config.py --compiler=gcc --compiler-bin=arm-none-eabi-gcc --config-output-lnt-file=%OUT_PATH%\co-arm-none-eabi-gcc.lnt --config-output-header-file=%OUT_PATH%\co-arm-none-eabi-gcc.h --generate-compiler-config

pause
