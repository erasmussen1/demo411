
project(rtc)

add_library(${PROJECT_NAME}
    STATIC
        src/rtc.c
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        src
)

target_link_libraries(${PROJECT_NAME}
    utils
)

# add_subdirectory(test)

