# Configuration

## IDE and SDK Configuration

- Install [STM32CubeIDE v1.11.0](https://www.st.com/content/ccc/resource/technical/software/sw_development_suite/group0/bf/f7/ee/25/04/54/48/2e/stm32cubeide_win/files/st-stm32cubeide_1.11.0_13638_20221122_1308_x86_64.exe.zip/jcr:content/translations/en.st-stm32cubeide_1.11.0_13638_20221122_1308_x86_64.exe.zip).
- Open STM32CubeIDE.
- Select the workspace "C:\Desarrollo\Proyectos\iertec_lib_stm32l4_ws" and click on launch.
- Allow firewall access to STM32CubeIDE.
- Help → Check for Updates → Install updates.
- Window → Preferencies → STM32Cube → Firmware Updater → Firmware installation repository. Set it to "C:/ST/STM32Cube/Repository/".
- Help → Manage Embedded Software Packages → STM32Cube MCU Packages → STM32L4. Install "STM32CubeL4 Firmware Package v1.17.2".
- Open script/global.sh and update the following variables:
    - PATH_IDE
    - PATH_TARGET_COMPILER
    - PATH_OPENOCD
    - PATH_OPENOCD_S

## Project Configuration

- Open script/global.sh and update the following variables:
    - PROJECT_NAME
    - ARTIFACT_NAME

### Project Creation

- Open STM32CubeIDE.
- File → New → STM32 Project.
- Go to "Board Selector" and select "NUCLEO-L452RE". Click on next.
- Setup STM32 Project:
    - Project Name: iertec_lib_stm32l4_test.
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
- Project → Properties → C/C++ Build → Settings → Tools Settings → MCU Settings → Use float with printf from newlib-nano (-u _printf_float). Mark the checkbox.
- Project → Properties → C/C++ Build → Settings → Tools Settings → MCU Post build outputs → Convert to Intel Hex file (-O ihex). Mark the checkbox.

### STM32CubeMX Configuration

- Open file "iertec_lib_stm32l4_test.ioc".
- Project options:
    - Project Manager → Project → Project Settings → Do not generate the main(). Mark the checkbox.
    - Project Manager → Code Generator → Generated files → Generate peripheral initialization as a pair of .c/.h files per peripheral. Mark the checkbox.
    - Project Manager → Project → Thread-safe Settings → Enable multi-threaded support. Mark the checkbox.
    - Project Manager → Project → Thread-safe locking strategy. Select "FreeRTOS Strategy #5 - Deny lock usage from interrupts".
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
- SPI configuration:
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
- FatFS configuration:
    - Pinout & Configuration → Middleware. Select FATFS.
    - Pinout & Configuration → FATFS Mode and Configuration → Mode. Check "User-defined".
    - Pinout & Configuration → FATFS Mode and Configuration → Configuration → Set Defines:
        - Function Parameters → USE_FIND. Select "Enabled".
        - Locale and Namespace Parameters → USE_LFN. Select "Enabled with dynamic worjking buffer on the STACK".
        - System Parameters → FS_EXFAT. Select "Enabled".
    - Pinout & Configuration → GPIO → GPIO Mode and COnfiguration → Configuration → SPI:
        - Select SPI1_MISO from the table.
        - GPIO Pull-up/Pull-down. Select Pull-up.
- Save the file.
- Click on "Yes" for the dialog "Do you want to generate Code?".

### Debug Configuration

Debug application in the target:
- Run → Debug Configurations... → STM32 C/C++ Application → Right click → New Configuration.
- Main → C/C++ Application → Search Project.... Select the Debug binary.
- Click on "Debug" button.

Debug test in the target:
- Run → Debug Configurations... → STM32 C/C++ Application → Right click → New Configuration.
- Name. Set it to "test_target_debug".
- Main → C/C++ Application. Set it with the path to the test binary to be debugged. For example, "C:\Desarrollo\Proyectos\iertec_lib_stm32l4_ws\script\output\test\integration_target_build\test\out\test_z_dummy.elf".
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
- Move file "test_host_debug.launch" from ".metadata/.plugins/org.eclipse.debug.core/.launches" to the project folder.
- Open Eclipse.

## MSYS2 Configuration

Many different packages from MSYS2 are used by the utility scripts.

- Install [MSYS2](https://github.com/msys2/msys2-installer/releases/download/2023-03-18/msys2-x86_64-20230318.exe).
- Select as installation folder "C:\Desarrollo\Programas\msys64".
- Launch "MSYS2 MSYS".
- Execute command "pacman -Syu" and accept the installation of the updates.
- Accept to close the terminal and relaunch "MSYS2 MSYS".
- Execute command "pacman -Syu" and accept the installation of the updates.
- Install the following packages:
    ```
    pacman -S mingw-w64-x86_64-toolchain
    pacman -S mingw-w64-x86_64-llvm
    pacman -S mingw-w64-x86_64-clang
    pacman -S mingw-w64-x86_64-clang-analyzer
    pacman -S mingw-w64-x86_64-clang-tools-extra
    pacman -S mingw-w64-x86_64-compiler-rt
    pacman -S mingw-w64-x86_64-libblocksruntime
    pacman -S mingw-w64-x86_64-cppcheck
    pacman -S mingw-w64-x86_64-python-pip
    pacman -S mingw-w64-x86_64-python-wheel
    pacman -S mingw-w64-x86_64-python-lxml
    pacman -S mingw-w64-x86_64-python-pyserial
    pacman -S mingw-w64-x86_64-python-regex
    pacman -S mingw-w64-x86_64-python-pillow
    pacman -S mingw-w64-x86_64-ruby
    pacman -S mingw-w64-x86_64-doxygen
    pacman -S mingw-w64-x86_64-qt6-base
    pacman -S mingw-w64-x86_64-graphviz
    pacman -S mingw-w64-x86_64-srecord
    pacman -S mingw-w64-x86_64-uncrustify
    pacman -S mingw-w64-x86_64-7zip
    ```
- Launch "MSYS2 MINGW64".
- Install the following python packages:
    ```
    pip install compiledb
    pip install gcovr
    pip install pyyaml
    ```
- Install the following ruby packages:
    ```
    gem install ceedling
    ```
- Ceedlin 0.31.1 contains some bugs. Perform the following modifications:
    - Open the file C:\Desarrollo\Programas\msys64\mingw64\lib\ruby\gems\3.1.0\gems\ceedling-0.31.1\bin\ceedling. Replace "YAML.load_file" with "YAML.unsafe_load_file".
    - Open the file C:\Desarrollo\Programas\msys64\mingw64\lib\ruby\gems\3.1.0\gems\ceedling-0.31.1\lib\ceedling\yaml_wrapper.rb. Replace "YAML.load" with "YAML.unsafe_load".
- Open script/launcher.bat and modify the MSYS2 installation path if necessary.

## Static Analysis Configuration

- Download PC-lint Plus and extact it in "C:\Desarrollo\Programas\pclp-X.Y.Z".
- Open script/static_analysis/do_analysis_clang_tidy.sh and update the following variables:
    - CLANG_TIDY_SRC_PATTERN
- Open script/static_analysis/do_analysis_pclint.sh and update the following variables:
    - PCLINT_SRC_PATTERN
- Open script/global.sh and update the following variables:
    - PATH_PCLINT
    - PATH_PCLINT_CONFIG
    - PATH_CPPCHECK_SHARE

## Tests Configuration

- Open the following files and modify them according to the project needs:
    - project_integration_target.yml
    - project_unit_host.yml
    - project_unit_target.yml

## Documentation Configuration

- Install [Java 17](https://adoptium.net/en-GB/temurin/releases/?version=17).
- Download [PlantUML](https://github.com/plantuml/plantuml/releases/download/v1.2023.5/plantuml-1.2023.5.jar).
- Copy the file in the folder "C:\Desarrollo\Programas\PlantUML" renaming it to "plantuml.jar".
- Open script/global.sh and update the following variables:
    - PATH_JAVA
    - PATH_PLANTUML
- Open script/doc/doxyfile with the application doxywizard and modify it according to the project needs.
- The application doxywizard can be found in "C:\Desarrollo\Programas\msys64\mingw64\bin\doxywizard.exe".

## Jenkins Configuration

- Install [Jenkins v2.375.3](https://get.jenkins.io/windows-stable/2.375.3/jenkins.msi).
- Open a browser and enter the URL http://localhost:8080/.
- Install suggested plugins.
- Create First Admin User.
    - User: admin
    - Password: admin
- Manage Jenkins → Manage Plugins → Availabe plugins. Search "Locale" and install it.
- Manage Jenkins → Configure System → Locale.
    - Default Language. Fill it with "en".
    - Ignore browser preference and force this language to all users. Mark the checkbox.
    - Click on "Save" button.
- Manage Jenkins → Manage Plugins → Availabe plugins. Search "Blue Ocean" and install it.
- Manage Jenkins → Manage Plugins → Availabe plugins. Search "xUnit" and install it.
- Manage Jenkins → Manage Plugins → Availabe plugins. Search "Cobertura" and install it.
- Manage Jenkins → Global Tool Configuration → Git → Path to Git executable. Fill it with "C:\Program Files\Git\bin\git.exe".
- Open "C:\Program Files\Jenkins\jenkins.xml" and add:
    ```xml
    <arguments>-Xrs -Xmx256m -
    Dhudson.lifecycle=hudson.lifecycle.WindowsServiceLifecycle -
    Dhudson.plugins.git.GitSCM.ALLOW_LOCAL_CHECKOUT=true -jar ...
    ```
- Restart Jenkins (restart PC).
- + New Item:
    - Enter an item name. Fill with the name for the pipeline, i.e., "iertec_lib_stm32l4".
    - Select the option "Pipeline".
    - Click on "OK".
    - Pipeline:
        - Definition. Select "Pipeline script from SCM".
        - SCM. Select "Git".
        - Repository URL. Fill with the path to the repository, i.e., "file://C:/Desarrollo/Proyectos/iertec_lib_stm32l4_ws".
        - Script Path. Fill with with the relative location to the Jenkins file, i.e., "script/ci_cd/Jenkinsfile".
        - Click on "Save".
- Open Blue Ocean.
    - Click on pipeline "iertec_lib_stm32l4".
    - Click on "Run".

## SonarQube Configuration

- Install [SonarQube 9.9](https://binaries.sonarsource.com/Distribution/sonarqube/sonarqube-9.9.0.65466.zip). Extract it in your programs folder.
- Install [SonarScanner 4.8](https://binaries.sonarsource.com/Distribution/sonar-scanner-cli/sonar-scanner-cli-4.8.0.2856.zip). Extract it in your programs folder.
- Copy [sonar-cxx 2.1.0](https://github.com/SonarOpenCommunity/sonar-cxx/releases/download/cxx-2.1.0/sonar-cxx-plugin-2.1.0.428.jar) into the folder ${sonarqube}/extensions/plugins/.
- Install [PostgreSQL 15.2](https://www.enterprisedb.com/downloads/postgres-postgresql-downloads).
    - Password: postgres
    - Port: 5432
- Open psql and execute the following commands:
    ```sql
    CREATE USER sonar;
    ALTER USER sonar WITH PASSWORD 'sonar';
    CREATE DATABASE sonardb WITH ENCODING 'UTF8';
    ALTER DATABASE sonardb OWNER TO sonar;
    ALTER USER sonar SET search_path TO sonardb;
    ```
- Open file ${sonarqube}/conf/sonar.properties and add:
    ```
    sonar.jdbc.username=sonar
    sonar.jdbc.password=sonar
    sonar.jdbc.url=jdbc:postgresql://localhost:5432/sonardb?currentSchema=public
    ```
- Execute script/ci_cd/do_sonarqube_server.bat to lauch the server.
    - Wait until server is ready (SonarQube is operational).
    - Maintain the command line opened while using SonarQube.
    - To stop the server pulse ctrl+c.
- Open a browser and enter the URL http://localhost:9000/.
- Login.
    - User: admin
    - Password: admin
    - New password: sonar
- Login with user admin and password admin. Set as new password sonar.
- Go to "Quality Profiles".
    - Search CXX.
    - Pulse the gear and select "Copy".
    - Give it the name JMFM.
- Go to "Rules".
    - In "Quality Profiles" select the created profile and set it in "inactive" mode.
    - Pulse "Clear all Filters".
    - In "Language", select CXX. Pulse "Bulk change" → "Activate In..." → Created profile → Apply.
- Go to "Quality Profiles".
    - Search the created profile.
    - Pulse the gear and select "Set as Default".
- User → My Account → Security → Generate Tokens:
    - Name: JMFM.
    - Type: User Token.
    - Expires in: No expiration.
    - Click on Generate.
    - Copy the code an paste it in the sonar-project.properties filed sonar.login.
- Execute script/ci_cd/do_sonar_scanner.bat to lauch the analysis.
