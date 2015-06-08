project(bpfs)
cmake_minimum_required(VERSION 2.8.11)

add_subdirectory(file-access)
add_subdirectory(core)
add_subdirectory(manager)
add_subdirectory(fuse-wrapper)
