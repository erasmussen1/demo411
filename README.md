# FreeRTOS project for STM32F411 Black Pill

This project targets the STM32F411CEU6-based Black Pill board (512 KiB flash,
128 KiB SRAM, Cortex-M4F at 100 MHz).

## Build

Before compiling, update `compile_path` in `cmake/toolchain_arm.cmake` to the
location of the Arm GNU Toolchain on your system.

```bash
./scripts/build.sh -dceA
```

The build produces `build/DemoRTOSProject.elf` and
`build/DemoRTOSProject.bin`.

To build the project and run all unit-test:
```bash
./scripts/build.sh -dceAG
```


## Flash with an ST-Link V2

### Requirements

- An ST-Link V2 programmer that supports SWD
- Four female-to-female jumper wires (five if using the optional reset wire)
- Either OpenOCD or the `st-flash` utility from stlink-tools

### Connect the programmer

Disconnect USB and any other power source from the Black Pill before wiring it.
Connect the ST-Link V2 to the Black Pill's SWD header as follows:

| ST-Link V2 | Black Pill | Purpose |
| --- | --- | --- |
| `3.3V` | `3V3` | Target voltage reference and power |
| `GND` | `GND` | Common ground |
| `SWDIO` | `DIO` / `PA13` | SWD data |
| `SWCLK` | `CLK` / `PA14` | SWD clock |
| `NRST` | `NRST` | Reset; optional, but useful for recovery |


STM32F411 Black Pill, the reset connection is the pin marked R on the top row.
```
... A2   A1   A0   R   C15   C14   C13   VB
                  ^
                  |
                 NRST
```

The connection is:
```
ST-LINK/V2             Your Black Pill
---------------------------------------
Pin 1   VTref   -----> 3.3
Pin 7   SWDIO   -----> SWDIO
Pin 9   SWCLK   -----> SWCLK
Pin 15  NRST    -----> R
Pin 20  GND     -----> GND
```

Note: The board can be powered via the USB-c connector.

Check the labels printed on the programmer rather than relying on connector pin
position, because ST-Link V2 clone pinouts vary. Use only 3.3 V; do not connect
the programmer's 5 V pin. If the board is powered from another source, leave
the ST-Link `3.3V` power output disconnected only if the programmer can still
sense the target's 3.3 V reference. Always connect ground.

Leave `BOOT0` low for normal startup from flash. After checking the wiring,
connect the ST-Link V2 to the computer.

### Flash with OpenOCD

From the project root, program the ELF image, verify it, reset the MCU, and exit:

```bash
openocd -f interface/stlink.cfg -f target/stm32f4x.cfg \
  -c "program build/DemoRTOSProject.elf verify reset exit"
```

A successful operation ends with messages indicating that the image was
written and verified. The ELF file contains the correct flash address, so no
address is required on the command line.

### Flash with st-flash

As an alternative, program the raw binary at the STM32F411 flash base address:

```bash
st-flash --reset write build/DemoRTOSProject.bin 0x08000000
```

Do not omit `0x08000000` when flashing the `.bin` file; unlike the ELF file, a
raw binary does not contain its destination address.

### Troubleshooting

- If the programmer cannot find the target, unplug it and recheck `3V3`, `GND`,
  `SWDIO`, and `SWCLK`. Also verify that no 5 V connection is present.
- If connection is intermittent, shorten the jumper wires and add the `NRST`
  connection.
- If user firmware prevents a normal debug connection, hold the Black Pill's
  reset button, start the flashing command, and release reset when the tool
  begins connecting.
- If Linux reports a USB permission error, run the command once with suitable
  device permissions or install the ST-Link udev rules supplied by the chosen
  flashing tool. Unplug and reconnect the programmer after changing rules.

## Build requirements

ARM cross compiler e.g. arm-gnu-toolchain-13.3.rel1-x86_64-arm-none-eabi

The toolchain can be downloaded from:
```
https://developer.arm.com/tools-and-software/gnu-toolchain
```
cmake version 3.20 or later


## Directory tree

```
.
├── cmake
│   ├── ProjectVersion.h.in
│   ├── toolchain_arm.cmake
│   └── toolchain_clang.cmake
├── CMakeLists.txt
├── doc
│   └── user_guide.md
├── include
│   └── FreeRTOSConfig.h
├── LICENSE
├── scripts
│   └── build.sh
├── src
│   ├── main.c
│   ├── startup_stm32f411xe.s
│   └── system_stm32f411xe.c
└── STM32F411CEUX_FLASH.ld

```
