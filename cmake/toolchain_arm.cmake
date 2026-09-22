
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

# set(compile_path "/opt/gcc-arm-none-eabi-10.3-2021.10")
set(compile_path "/opt/arm-gnu-toolchain-13.3.rel1-x86_64-arm-none-eabi")

set(CMAKE_C_COMPILER   "${compile_path}/bin/arm-none-eabi-gcc")
set(CMAKE_CXX_COMPILER "${compile_path}/bin/arm-none-eabi-g++")
# Use the GCC driver for assembly sources so CMake's compile definitions and
# target CPU flags are translated consistently with the C/C++ sources.
set(CMAKE_ASM_COMPILER "${compile_path}/bin/arm-none-eabi-gcc")


# Find the rest of the embedded programs
find_program(GDB      arm-none-eabi-gdb)
find_program(OPENOCD  openocd)
find_program(OBJCOPY  arm-none-eabi-objcopy)
find_program(OBJDUMP  arm-none-eabi-objdump)
find_program(SIZE     arm-none-eabi-size)
find_program(OBJNAMES arm-none-eabi-nm)
find_program(ARCHIVE  arm-none-eabi-ar)
find_program(STRIP    arm-none-eabi-strip)
find_program(STRINGS  arm-none-eabi-strings)
find_program(GCOV     arm-none-eabi-gcov)

set(CMAKE_C_COMPILER_ID GNU)
set(CMAKE_CXX_COMPILER_ID GNU)

set(CMAKE_C_COMPILER_FORCED TRUE)
set(CMAKE_CXX_COMPILER_FORCED TRUE)
