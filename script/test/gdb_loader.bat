@echo off

start arm-none-eabi-gdb -batch-silent -x ../script/test/gdb_loader.gdb -se %1
openocd -d0 -f %PROJECT_NAME%_openocd.cfg -s %PATH_OPENOCD_S% -c "gdb_report_data_abort enable" -c "gdb_port 3333" -c "tcl_port 6666" -c "telnet_port 4444"
call %~dp0..\coverage\gcov_partial.bat ..\script\output\test\integration_target_build\test\out\c target_%~n1
