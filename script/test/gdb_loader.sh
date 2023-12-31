
(arm-none-eabi-gdb -batch-silent -x ../script/test/gdb_loader.gdb -se $1 &
 openocd -d0 -f test_target_debug.cfg -s $PATH_OPENOCD_S -c "gdb_report_data_abort enable" -c "gdb_port 3333" -c "tcl_port 6666" -c "telnet_port 4444") 2>&1 | 
grep -v -f ../script/test/gdb_loader_filter.txt

wait

bash $PATH_COVERAGE/gcov_target.sh $(dirname "$1") $2_$(basename "$1")
