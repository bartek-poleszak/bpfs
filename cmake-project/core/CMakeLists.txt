aux_source_directory(. SRC_LIST)
set(HEADER_FILES bpfstypes.h)

add_definitions(
    -std=c++11
    -Wall
)

add_library(core ${SRC_LIST} ${HEADER_FILES})
#add_library(core filedisk.cpp)
target_include_directories(core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

#add_executable(core ${SRC_LIST})
#add_executable(core filedisk.cpp idisk.cpp)
target_link_libraries(core LINK_PUBLIC FileAccess)
