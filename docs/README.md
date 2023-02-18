# Readme

## About the Project

Generic library to be used with STM32L4 microcontrollers.

## Dependencies

- IDE: [STM32CubeIDE v1.11.0](https://www.st.com/content/ccc/resource/technical/software/sw_development_suite/group0/bf/f7/ee/25/04/54/48/2e/stm32cubeide_win/files/st-stm32cubeide_1.11.0_13638_20221122_1308_x86_64.exe.zip/jcr:content/translations/en.st-stm32cubeide_1.11.0_13638_20221122_1308_x86_64.exe.zip).
- SDK: STM32CubeL4 Firmware Package v1.17.2. Installed from the IDE.
- RTOS: FreeRTOS v10.3.1. Available in the SDK.

## IDE and SDK Configuration

- Open STM32CubeIDE.
- Select the workspace "C:\Desarrollo\Proyectos\iertec_lib_stm32l4_ws" and click on launch.
- Allow firewall access to STM32CubeIDE.
- Help → Check for Updates → Install updates.
- Window → Preferencies → STM32Cube → Firmware Updater → Firmware installation repository. Set it to "C:/ST/STM32Cube/Repository/".
- Help → Manage Embedded Software Packages → STM32Cube MCU Packages → STM32L4. Install "STM32CubeL4 Firmware Package v1.17.2".

## Project Configuration

### Project Creation

- Open STM32CubeIDE.
- File → New → STM32 Project.
- Go to "Board Selector" and select "NUCLEO-L452RE". Click on next.
- Setup STM32 Project:
    - Project Name: iertec_lib_stm32l4_test_mx.
    - Targeted Language: C.
    - Targeted Binary Type: Executable.
    - Targeted Project Type: STM32Cube.
- Finish.
- Click on "Yes" for the dialog "Initialize all peripherals with their default Mode?".
- Project → Properties → Configuration. Select "All configurations".
- Project → Properties → C/C++ General → Paths and Symbols:
    - Includes → Languages → GNU C → Add:
        - src
        - lib/iertec_lib_stm32l4/fsm
        - lib/iertec_lib_stm32l4/itf
        - lib/iertec_lib_stm32l4/rtc
        - lib/iertec_lib_stm32l4/rtos
        - lib/iertec_lib_stm32l4/task
        - lib/iertec_lib_stm32l4/util
    - Source Location → Add Folder...:
        - src
        - lib

### STM32CubeMX Configuration

- Open file "iertec_lib_stm32l4_test_mx.ioc".
- Project options:
    - Project Manager → Project → Project Settings → Do not generate the main(). Mark the checkbox.
    - Project Manager → Code Generator → Generated files → Generate peripheral initialization as a pair of .c/.h files per peripheral. Mark the checkbox.
- Debug configuration:
    - Pinout & Configuration → System Core → SYS → SYS Mode and Configuration → Mode → Debug. Select "Serial Wire".
- Main clock configuration:
    - Pinout & Configuration → System Core → RCC → Low Speed Clock (LSE). Select "Crystal/Ceramic Resonator".
    - Clock Configuration → MSI RC. Select "48000" kHz.
    - Clock Configuration → System Clock Mux. Select "MSI".
- HAL timer configuration:
    - Pinout & Configuration → System Core → SYS → Timebase Source. Select "TIM6".
- FreeRTOS configuration:
    - Pinout & Configuration → Middleware → FreeRTOS:
        - Mode → Interface. Select "CMSIS_V1".
        - Configuration → Advanced settings → USE_NEWLIB_REENTRANT. Select "Enabled".
        - Configuration → Config Parameters → ENABLE_FPU. Select "Enabled".
        - Configuration → Config Parameters → USE_COUNTING_SEMAPHORES. Select "Enabled".
        - Configuration → Config Parameters → USE_TICKLESS_IDLE. Select "User defined functionality enabled".
        - Configuration → Config Parameters → Memory Allocation. Select "Dynamic".
        - Configuration → Config Parameters → TOTAL_HEAP_SIZE. Set to "10000" bytes.
        - Configuration → Config Parameters → USE_MALLOC_FAILED_HOOK. Select "Enabled".
        - Configuration → Config Parameters → CHECK_FOR_STACK_OVERFLOW. Select "Option 2".
        - Configuration → Config Parameters → USE_TRACE_FACILITY. Select "Enabled".
        - Configuration → Config Parameters → USE_TIMERS. Select "Enabled".
        - Configuration → Include parameters → vTaskDelayUntil. Select "Enabled".
- RTC timer configuration:
    - Pinout & Configuration → Timers → LPTIM2:
        - Mode → Mode. Select "Counts internal clock events".
        - Configuration → NVIC Settings → LPTIM2 global interrupt. Mark the checkbox "Enabled".
    - Clock Configuration → LPTIM2 Clock Mux. Select "LSE".
- Watchdog configuration:
    - Pinout & Configuration → System Core → IWDG → Mode. Mark the checkbox "Activated".
- Debug USART configuration:
    - Pinout & Configuration → Connectivity → UASRT2:
        - UART2 Mode and Configuration → Mode → Mode. Select "Asynchronous".
        - NVIC Settings → USART2 global interrupt. Mark the checkbox "Enable".
    - Clock Configuration → USART2 Clock Mux. Select "HSI".
    - Pinout & Configuration → Pinout view → PA15 (JTDI). Select "GPIO_input".
    - Pinout & Configuration → System Core → GPIO → GPIO → PA15 (JTDI):
        -  GPIO Pull-up/Pull-down. Select "Pull-down".
        -  User Label. Fill "TDI".
- USART with flow control configuration:
    - Pinout & Configuration → Connectivity → USART1:
        - Mode → Mode. Select "Asynchronous".
        - Mode → Hardware Flow Control (RS232). Select "CTS Only".
        - Configuration → NVIC Settings → USART1 global interrupt. Mark the checkbox "Enable".
    - Pinout & Configuration → Pinout view → PA12. Select "GPIO_output".
    - Pinout & Configuration → System Core → GPIO → GPIO → PA12:
        - Maximum output speed. Select "Very High".
        - User Label. Fill "USART1_RTS".
    - Clock Configuration → USART1 Clock Mux. Select "HSI".
-  SPI configuration:
    - Pinout & Configuration → Connectivity → SPI1:
        - Mode. Select "Full-Duplex Master".
        - Configuration → Parameter Settings → Basic Parameters → Data Size. Select "8 Bits".
        - Configuration → Parameter Settings → Clock Parameters → Prescaler (for Baud Rate). Select "8".
        - Configuration → Parameter Settings → Advanced Parameters → NSSP Mode. Select "Disabled".
        - Configuration → Parameter Settings → DMA Settings → Add → DMA Request. Select SPI1_TX.
        - Configuration → Parameter Settings → DMA Settings → Add → DMA Request. Select SPI1_RX. Select Priority "High".
    - Pinout & Configuration → Pinout view → PA0. Select "GPIO_output".
    - Pinout & Configuration → System Core → GPIO → GPIO → PA0:
        - GPIO output level. Select "High".
        - Maximum output speed. Select "High".
        - User Label. Fill "SPI_CS".
- GPIO configuration:
    - Pinout & Configuration → Pinout view → PB1. Select "GPIO_output".
    - Pinout & Configuration → System Core → GPIO → GPIO → PB1:
        - Maximum output speed. Select "Very High".
        - User Label. Fill "IO_OUT_1".
    - Pinout & Configuration → Pinout view → PA8. Select "GPIO_EXTI8".
    - Pinout & Configuration → System Core → GPIO → GPIO → PA8:
        - GPIO mode. Select "External Interrupt Mode with Rising/Falling edge trigger detection".
        - User Label. Fill "IO_IN_1".
    - Pinout & Configuration → System Core → GPIO → NVIC → EXTI line [9:5] interrupts. Mark the checkbox "Enabled".
- I2C configuration:
    - Pinout & Configuration → Connectivity → I2C1:
        - Mode → I2C. Select "I2C".
        - Configuration → Parameter Settings → Timing configuration:
            - I2C Speed Mode. Select "Fast Mode".
            - Rise time (ns). Set to "100".
            - Fall time (ns). Set to "10".
        - Configuration → DMA Settings → Add. Select "I2C1_TX".
        - Configuration → DMA Settings → Add. Select "I2C1_RX".
        - Configuration → NVIC Settings → I2C1 event interrupt. Mark the checkbox "Enabled".
        - Configuration → NVIC Settings → I2C1 error interrupt. Mark the checkbox "Enabled".
        - Configuration → GPIO Settings → I2C1_SCL → GPIO Pull-up/Pull-down. Select "Pull-up".
        - Configuration → GPIO Settings → I2C1_SDA → GPIO Pull-up/Pull-down. Select "Pull-up".
        - Relocate PB6 to PB8 and PB7 to PB9.
    - Clock Configuration → I2C1 Clock Mux. Select "HSI".
- Save the file.
- Click on "Yes" for the dialog "Do you want to generate Code?".

### Debug Configuration

Debug application in the target:
- Run → Debug Configurations... → STM32 C/C++ Application → Right click → New Configuration.
- Main → C/C++ Application -> Search Project.... Select the Debug binary.
- Click on "Debug" button.

Debug test in the target:
- Run → Debug Configurations... → STM32 C/C++ Application → Right click → New Configuration.
- Name. Set it to "test_target_debug".
- Main → C/C++ Application. Set it with the path to the test binary to be debugged. For example, "C:\Desarrollo\Proyectos\iertec_lib_stm32l4_ws\script\output\test\integration_target_build\test\out\test_itf_uart.elf".
- Main → Build (if required) before launching. Select "Disable auto build".
- Debugger → Debug Probe. Select "ST-LINK (OpenOCD)".
- Startup → Initialization commands. Fill it with "monitor arm semihosting enable".
- Click on "Debug" button.

Debug test in the host:
- Run → Debug Configurations... → C/C++ Application → Right click → New Configuration.
- Name. Set it to "test_host_debug".
- Main → C/C++ Application. Set it with the path to the test binary to be debugged. For example, "C:\Desarrollo\Proyectos\iertec_lib_stm32l4_ws\script\output\test\unit_host_build\test\out\test_z_dummy.exe".
- Main → Build (if required) before launching. Select "Disable auto build".
- Debugger → Main → GDB Debugger. Set it with the path to the debugger. For example, "C:\Desarrollo\Programas\msys64\mingw64\bin\gdb.exe".
- Click on "Debug" button.
- Close Eclipse.
- Move file "test_host_debug.launch" from ".metadata\.plugins\org.eclipse.debug.core\.launches" to the project folder.
- Open Eclipse.
