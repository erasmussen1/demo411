
project(menu)

add_library(${PROJECT_NAME}
    STATIC
        src/menu_util.c
        src/amenu.c
        src/bmenu.c
        src/tmenu.c
        src/menu.c
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        src
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        utils
        rtc
)

add_subdirectory(test)

