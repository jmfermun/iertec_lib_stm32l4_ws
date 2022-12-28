target extended-remote localhost:3333

monitor arm semihosting enable
monitor reset halt
load
monitor reset init

break _exit
commands
monitor shutdown
quit 0
end

continue