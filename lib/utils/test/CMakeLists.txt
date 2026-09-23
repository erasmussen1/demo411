
project(test_utils LANGUAGES CXX)

add_executable(${PROJECT_NAME}
    test_utils.cpp
)

target_link_libraries(${PROJECT_NAME}
    utils
    gt2test
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    SUFFIX ".elf"
)

target_link_options(${PROJECT_NAME}
    PRIVATE
        -mcpu=${PROJ_CPU}
        -mfloat-abi=${PROJ_FLOAT_ABI}
        -mfpu=${PROJ_MPU}
        -mthumb
        -ffunction-sections
        -fdata-sections
        -g
        -specs=rdimon.specs
)

add_test(NAME ${PROJECT_NAME}
    COMMAND qemu-arm-static -cpu ${PROJ_CPU} ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.elf
)

