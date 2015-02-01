set(SRC_LIST
    main.cpp
)

add_definitions(
    -std=c++11
    -Wall
)

#add_executable(manager main.cpp)
add_executable(tests ${SRC_LIST})
target_link_libraries(tests LINK_PUBLIC core)
