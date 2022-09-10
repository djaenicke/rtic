<p align="center">
  <h1 align="center">RTIC</h1>
</p>

### The Hardware
The Real Time Interface and Controller (RTIC) is a universal board for low-level robotics control and monitoring. The board can be ordered from [JLCPCB](https://jlcpcb.com/) using the [assembly](pcb/kicad/v2/assembly) and [gerber](pcb/kicad/v2/gerber) files.

#### Key Features
- 4 permanent magnet DC motor outputs
- 4 quadrature encoder inputs
- 6-axis IMU
- NRF24L01 socket for wireless communications
- USB (supports DFU via jumper)
- CAN breakout header

![](pcb/kicad/v2/rtic_v2.png?raw=true)

A [compatible 4-wheeled mobile robot base](https://www.amazon.com/Moebius-Mecanum-Platform-Omni-direactional-Raspberry/dp/B083XS2SD7) can be purchased directly from Amazon. :robot:

### Tools Setup ✔️
At a minium you will need the ARM GNU GCC Toolchain, Make and CMake to compile the source and generate executable artifacts.
-   [ARM GNU GCC Toolchain - 10-2020-q4-major](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads) - Other versions can be used but the current source is actively developed and tested using the 10-2020-q4-major release
-   Make -  Make is shipped with Linux and should already be present on the developers system. If the developer is running Windows they can either use WSL to run a Linux instance or install Make on Windows via [MinGW](https://sourceforge.net/projects/mingw/)
-   [CMake](https://cmake.org/download/)

To flash and debug the target this project uses a couple of different tools. PyOCD, Segger JLink, or STLINK can be used as the programming/debugging medium, and a VScode extension called Cortex-Debug gives a user interface within VScode for stepping through the source, settings breakpoints and viewing registers/variables within the target.
-   [PyOCD](https://github.com/pyocd/pyOCD#installing)
-   [JLink](https://www.segger.com/downloads/jlink/)
-   [STLINK](https://github.com/stlink-org/stlink)
-   [VScode](https://code.visualstudio.com/)
-   [ARM Developer Extension Pack for VScode](https://marketplace.visualstudio.com/items?itemName=Glasslabs.arm-developer-toolkit)

As an alternative to the debuggging tools, dfu-util can be used to program the board using the built in DFU capability of the bootloader.
-   [dfu-util](https://manpages.ubuntu.com/manpages/xenial/man1/dfu-util.1.html)
    - Linux - ```sudo apt-get install dfu-util```

***NOTE: After installing your desired debug toolset, please ensure the chosen application is accessible from the command line (pyocd or JLinkExe)***

To format all source files, you will need to install Clang-Format.
-   [Clang-Format](https://clang.llvm.org/docs/ClangFormat.html)
    - Linux - ```sudo apt-get install clang-format```

### Compiling, Flashing & Testing ✨
After a fresh clone, the ```setup``` scripts can be used to initialize the project. Passing ```release``` as an argument will generate a release build. Otherwise a ```debug``` build is generated.
##### Windows
```console
setup.ps1
```
##### Linux
```console
./setup.sh
```

Once a project has been initialized, the following commands should be ran from the ```build/``` folder

To clean the working directory:
```console
make clean
```

To compile the source:
```console
make -j8
```

To erase the connected target with PyOCD or Segger JLink:
```console
make erase
```

To flash the connected target with PyOCD or Segger JLink:
```console
make flash
```

To flash the connected target via DFU:
```console
make dfu
```

To execute unit tests:
```console
make test
```

To format source files:
```console
make format
```

### Source Documentation Style 📃
Read more about the Source documentation style [here](./doc/DOCUMENTATION_STYLE.md)

### Develop Strategy 📖
Read more about the Git develop strategy used [here](./doc/DEVELOP_STRATEGY.md)
