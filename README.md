# stm32-boilerplate

![](https://travis-ci.com/jaxxzer/stm32-boilerplate.svg?branch=master)

A minimal project to get going with stm32 microcontrollers.

This project depends on `arm-none-eabi-gcc`, `cmake`, and `python`. Continuous integration depends also on `clang-format` for style checks.

To install the toolchain and test your installation run:
```sh
./tools/travis-ci-script.sh
```

To build an example from the [example directory](/src/example) choose a [target board](/src/target) and do something like this:

```sh
mkdir build
cd build
cmake --configure -DTARGET_BOARD=nucleo-f303 -DEXAMPLE=pwm-output-dma ..
make
```

The program can be uploaded to the board with an stlink programmer by making the `flash` target:

```sh
make flash
```

visual studio code can be used to upload and debug the code with openocd and the cortex-debug plugin
