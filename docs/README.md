# Readme

## About the Project

Generic library to be used with STM32L4 microcontrollers.

## Software Dependencies

See [CONFIGURATION.md](CONFIGURATION.md) file for details.

## Hardware Dependencies

- Development board: [NUCLEO-L452RE](https://www.st.com/en/evaluation-tools/nucleo-l452re.html). [Purchase option](https://www.mouser.es/ProductDetail/STMicroelectronics/NUCLEO-L452RE?qs=zwwtnr6GuIyXyX5dw%2FgKaw%3D%3D).
- USB A to Mini-B cable.

## Getting the Source

This project is [hosted on GitHub](https://github.com/juanmanuelfernandez-iertec/iertec_lib_stm32l4_ws). You can clone this repository directly using this command:

```
git clone --recursive https://github.com/juanmanuelfernandez-iertec/iertec_lib_stm32l4_ws.git
```

## Building Release Binaries

Follow these steps to build the binaries:
- Open *STM32CubeIDE* and select the workspace *iertec_lib_stm32l4_ws*.
- Rebuild project *iertec_lib_stm32l4_test* (*Release* build configuration).

The following artifacts will be generated:
- iertec_lib_stm32l4_ws/iertec_lib_stm32l4_test/Release/iertec_lib_stm32l4_test.hex.

## Debugging

To debug the iertec_lib_stm32l4_test application, perform the following steps:
- Open the workspace *iertec_lib_stm32l4_ws* with *STM32CubeIDE*.
- Set active the *iertec_lib_stm32l4_test* project.
- Select the *Debug* build configuration.
- Rebuild the project.
- Load the project with the *STM32CubeIDE* debug button.
- You are ready for the debug session.

To see the debug information, perform the following steps:
- The programmer will create a virtual serial port. Use the one named *STMicroelectronics STlink Virtual COM Port*.
- The connection to the serial port must have the following configuration:
- 576000 bps.
- 8 data bits.
- 1 stop bit.
- No parity.
- No flow control.
- Open a terminal program (e.g. PuTTY).

## Tests

The following tools are used for the code testing:
- [Ceedling](https://github.com/ThrowTheSwitch/Ceedling) as the testing framework.
- [OpenOCD](https://openocd.org/) and [GDB](https://www.sourceware.org/gdb/) to load and execute the tests on the target.
- [Gcov](https://gcc.gnu.org/onlinedocs/gcc/Gcov.html) and [Gcovr](https://gcovr.com/en/stable/) to generate coverage information from the tests execution.

Three different types of tests are supported:
- Integration tests on the target.
- Unit tests on the host.
- Unit tests on the target.

## Coding Standard

- Try to follow [BARR-C](https://barrgroup.com/sites/default/files/barr_c_coding_standard_2018.pdf) coding standard.
- Use script/format/do_format.bat to perform a code formatting. It uses [Uncrustify](https://github.com/uncrustify/uncrustify).
- Modify the file do_format.sh to perform the formatting only in the files that you desire.
- The Uncrustify configuration file tries to follow the above standard. The tool is not perfect, but helps a lot.
- For the source code documentation use the [Doxygen](https://www.doxygen.nl/) format for the C language.

## License

See [LICENSE.md](LICENSE.md) file for details.

## Authors

- **[Juan Manuel Fernández Muñoz](https://www.linkedin.com/in/juan-manuel-fern%C3%A1ndez-mu%C3%B1oz-668a43202)** - *Initial work*.
