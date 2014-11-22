aux_source_directory(. SRC_LIST)

add_definitions(
    -std=c++11
    -Wall
)

#add_executable(manager main.cpp)
add_executable(manager ${SRC_LIST})
target_link_libraries(manager LINK_PUBLIC core)
