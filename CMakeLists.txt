cmake_minimum_required(VERSION 3.25)

project(DemoRTOSProject
    VERSION 1.1.0
    LANGUAGES C CXX ASM
)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 26)

option(RELEASE_BUILD "Release build " OFF)
option(BUILD_UNITTEST "Build unittests " OFF)
option(ENABLE_SANITIZER "Enable address sanitizer" OFF)
option(RUN_CLANG_TIDY "Run clang-tidy" OFF)
option(ENABLE_CPPCHECK "Enable cppcheck" OFF)

# Switch to enable a more verbose output for the gmake useful for debugging)
set(CMAKE_VERBOSE_MAKEFILE OFF)

# Create json meta data for Visual Studio Code, clang-tidy, code complete and etc
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

if (BUILD_UNITTEST)
    message(WARNING "  === Building unit-tests === ")
    enable_testing()
endif()

if(ENABLE_CPPCHECK)
    message(WARNING "  === cppcheck build === ")
    set(CMAKE_CXX_CPPCHECK "cppcheck")
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

# Get the current working branch from git
execute_process(
    COMMAND git rev-parse --abbrev-ref HEAD
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_BRANCH
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

# Get the latest abbreviated commit hash of the working branch
execute_process(
    COMMAND git log -1 --format=%h
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_COMMIT_HASH
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

configure_file(
    ${CMAKE_SOURCE_DIR}/cmake/ProjectVersion.h.in
    ${CMAKE_BINARY_DIR}/ProjectVersion.h
)

include_directories(
    ${CMAKE_BINARY_DIR}
    src
)

add_library(gt2test INTERFACE)
target_include_directories(gt2test
    INTERFACE
        gt2test
)

set(PROJ_CPU cortex-m4)
set(PROJ_MPU fpv4-sp-d16)
set(PROJ_FLOAT_ABI hard)

# Add the freertos_config for FreeRTOS-Kernel
add_library(freertos_config INTERFACE)
target_include_directories(freertos_config
    INTERFACE
        include
)

if (DEFINED FREERTOS_SMP_EXAMPLE AND FREERTOS_SMP_EXAMPLE STREQUAL "1")
    message(STATUS "Build FreeRTOS SMP example")
    # Adding the following configurations to build SMP template port
    add_compile_options( -DconfigNUMBER_OF_CORES=2 -DconfigUSE_PASSIVE_IDLE_HOOK=0 )
endif()

# Select the heap port.  values between 1-4 will pick a heap.
set(FREERTOS_HEAP "4" CACHE STRING "" FORCE)

# Select the native compile PORT
set(FREERTOS_PORT "GCC_ARM_CM4F" CACHE STRING "" FORCE)

# add_compile_definitions(
#     MY_GLOBAL_VAR
# )

if (RELEASE_BUILD)
    add_compile_options(-DNODEBUG -Os -O3 -g3 )
else()
    add_compile_options(-DDEBUG -O0 -g3 )
endif()

add_compile_options(
    $<$<COMPILE_LANGUAGE:CXX>:-Wno-exceptions>
    $<$<COMPILE_LANGUAGE:C>:-Wmissing-prototypes>
    $<$<COMPILE_LANGUAGE:CXX>:-Wcast-qual>
    -Wpointer-arith
    -Wcast-qual
    -Wsign-compare
    -Wunused-variable
    -Wunused-value
    -Wunused-label
    -Wunused-but-set-variable
    -Wparentheses
    -Wreturn-type
    -Wno-unused-function
    -fstack-protector-all
    -Wshift-count-overflow
    -Wpedantic
    -Wconversion
    -Wsign-conversion
    -Wshadow
    -Wnull-dereference
    -Wdouble-promotion
    -Wimplicit-fallthrough
    -fstack-protector-strong
    -fstack-clash-protection
)

add_compile_options(
    -mcpu=${PROJ_CPU}
    -mthumb
    -mfpu=${PROJ_MPU}
    -mfloat-abi=${PROJ_FLOAT_ABI}
)

# Adding the FreeRTOS-Kernel subdirectory
add_subdirectory(FreeRTOS-Kernel)

target_compile_options(freertos_kernel
    PRIVATE
    ### Gnu/Clang C Options
    $<$<COMPILE_LANG_AND_ID:C,GNU>:-fdiagnostics-color=always>
    $<$<COMPILE_LANG_AND_ID:C,Clang>:-fcolor-diagnostics>

    $<$<COMPILE_LANG_AND_ID:C,Clang,GNU>:-Wall>
    $<$<COMPILE_LANG_AND_ID:C,Clang,GNU>:-Wextra>
    $<$<COMPILE_LANG_AND_ID:C,Clang,GNU>:-Wpedantic>
    $<$<COMPILE_LANG_AND_ID:C,Clang,GNU>:-Werror>
    $<$<COMPILE_LANG_AND_ID:C,Clang,GNU>:-Wconversion>
    $<$<COMPILE_LANG_AND_ID:C,Clang>:-Weverything>

    # Suppressions required to build clean with clang.
    $<$<COMPILE_LANG_AND_ID:C,Clang>:-Wno-unused-macros>
    $<$<COMPILE_LANG_AND_ID:C,Clang>:-Wno-padded>
    $<$<COMPILE_LANG_AND_ID:C,Clang>:-Wno-missing-variable-declarations>
    $<$<COMPILE_LANG_AND_ID:C,Clang>:-Wno-covered-switch-default>
    $<$<COMPILE_LANG_AND_ID:C,Clang>:-Wno-cast-align>
)

# add_compile_options(-gdwarf-5)

add_subdirectory(lib)

# syscalls.c is needed because we use nosys.specs
add_executable(${PROJECT_NAME}
    src/startup_stm32f411xe.s
    src/system_stm32f411xe.c
    src/syscalls.c
    src/main.c
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    SUFFIX ".elf"
)

target_link_libraries(${PROJECT_NAME}
    freertos_kernel
    freertos_config
    utils
    rtc
    menu
)

set_property(TARGET
    freertos_kernel
        PROPERTY
            C_STANDARD 90
)

target_link_options(${PROJECT_NAME}
    PRIVATE
    "LINKER:-Map,$<TARGET_FILE_DIR:${PROJECT_NAME}>/$<TARGET_FILE_BASE_NAME:${PROJECT_NAME}>.map"
    "LINKER:-T,${CMAKE_SOURCE_DIR}/src/STM32F411CEUX_FLASH.ld"
    "LINKER:--gc-sections"
    "LINKER:-z,noexecstack"
    -mcpu=${PROJ_CPU}
    -mfloat-abi=${PROJ_FLOAT_ABI}
    -mfpu=${PROJ_MPU}
    -mthumb
    -ffunction-sections
    -fdata-sections
    -g
)

# Provide the bare-metal system-call stubs used by newlib.
#     Semi-hosting syscalls through debugger
target_link_options(${PROJECT_NAME}
    PRIVATE
      -specs=nosys.specs
)

# Print size after build
add_custom_command(TARGET ${PROJECT_NAME}
    POST_BUILD
        COMMAND ${SIZE} -x -G ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.elf
        COMMENT "Invoking: Cross ARM GNU Print Size"
)

if(CMAKE_OBJCOPY)
    set(PROJECT_OBJCOPY "${CMAKE_OBJCOPY}")
elseif(OBJCOPY)
    set(PROJECT_OBJCOPY "${OBJCOPY}")
else()
    find_program(PROJECT_OBJCOPY NAMES arm-none-eabi-objcopy objcopy REQUIRED)
endif()

if(CMAKE_OBJDUMP)
    set(PROJECT_OBJDUMP "${CMAKE_OBJDUMP}")
elseif(OBJDUMP)
    set(PROJECT_OBJDUMP "${OBJDUMP}")
else()
    find_program(PROJECT_OBJDUMP NAMES arm-none-eabi-objdump objdump REQUIRED)
endif()

add_custom_command(TARGET ${PROJECT_NAME}
    POST_BUILD
        COMMAND /bin/sh -c
            "\"${PROJECT_OBJDUMP}\" -h -S \"$<TARGET_FILE:${PROJECT_NAME}>\" > \"$<TARGET_FILE_DIR:${PROJECT_NAME}>/$<TARGET_FILE_BASE_NAME:${PROJECT_NAME}>.lst\""
        COMMENT "Generating disassembly listing for $<TARGET_FILE_NAME:${PROJECT_NAME}>"
        VERBATIM
)

add_custom_command(TARGET ${PROJECT_NAME}
    POST_BUILD
    	COMMAND "${PROJECT_OBJCOPY}" -O binary
            "$<TARGET_FILE:${PROJECT_NAME}>"
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>/$<TARGET_FILE_BASE_NAME:${PROJECT_NAME}>.bin"
    	COMMENT "Creating raw binary image from $<TARGET_FILE_NAME:${PROJECT_NAME}>"
    	VERBATIM
)
