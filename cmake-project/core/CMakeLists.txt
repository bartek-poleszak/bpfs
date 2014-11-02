aux_source_directory(. SRC_LIST)

add_definitions(
    -std=c++11
)

add_executable(core ${SRC_LIST})
#add_executable(core filedisk.cpp idisk.cpp main.cpp)
target_link_libraries(core LINK_PUBLIC FileAccess)
