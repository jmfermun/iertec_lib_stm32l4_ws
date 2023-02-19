@echo off

cd /d %~dp0

call ..\global.bat

set PATH=%PATH_MSYS%;%PATH%
set PATH=%PATH_TARGET_COMPILER%;%PATH%
set BUILD_PATH=.\..\..\%PROJECT_NAME%\Release
set OUT_PATH=.\..\output\static_analysis

rem PC-Lint analysis
set PCLINT_SRC_PATTERN=".*/../src/.*|.*/iertec_lib_cc26xx/.*"

python %PATH_PCLINT_CONFIG%\pclp_config.py --compiler=gcc --compiler-bin=arm-none-eabi-gcc --compilation-db=%BUILD_PATH%\compile_commands.json --config-output-lnt-file=%OUT_PATH%\co-project.lnt --generate-project-config --source-pattern=%PCLINT_SRC_PATTERN%
%PATH_PCLINT%\pclp64.exe %OUT_PATH%\co-arm-none-eabi-gcc.lnt pclint_arg.lnt %OUT_PATH%\co-project.lnt > %OUT_PATH%\report_pclint.txt

pause
