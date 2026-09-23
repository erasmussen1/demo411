
project(utils)

add_library(${PROJECT_NAME}
    STATIC
        src/utils.c
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        src
)

add_subdirectory(test)

