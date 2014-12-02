aux_source_directory(. SRC_LIST)
set(HEADER_FILES menuoptions.h)

add_definitions(
    -std=c++11
    -Wall
)

#add_executable(manager main.cpp)
add_executable(manager ${SRC_LIST} ${HEADER_FILES})
target_link_libraries(manager LINK_PUBLIC core)
