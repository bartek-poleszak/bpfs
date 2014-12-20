set(SRC_LIST
    main.cpp
    utils.cpp
)

add_definitions(
    -std=c++11
    -Wall
    `pkg-config fuse --cflags`
)

find_package(Threads REQUIRED)
add_executable(fuse-wrapper ${SRC_LIST})
target_link_libraries(fuse-wrapper
    LINK_PUBLIC core
    fuse
    ${CMAKE_THREAD_LIBS_INIT}
)
