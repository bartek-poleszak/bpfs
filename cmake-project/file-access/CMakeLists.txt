add_library(FileAccess blockDevice.c)
target_include_directories(FileAccess PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
