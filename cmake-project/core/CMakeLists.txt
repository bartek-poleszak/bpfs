set(HEADER_FILES
    bpfstypes.h
)
set(SRC_LIST
    file.cpp
    directory.cpp
    rootdirectory.cpp
    filedisk.cpp
    fscreator.cpp
    fspartition.cpp
    idisk.cpp
    ifreeblockmanager.cpp
    inode.cpp
    inodetable.cpp
    partitionheader.cpp
    permissions.cpp
    rawdatautils.cpp
    simplefreeblockmanager.cpp
    log.cpp
    indirectblock.cpp
    iindirectblock.cpp
    diskmatrix.cpp
    listfreeblockmanager.cpp
    encryptor.cpp
    xorencryptor.cpp
)

add_definitions(
    -std=c++11
    -Wall
)

add_library(core ${SRC_LIST} ${HEADER_FILES})
target_include_directories(core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(core LINK_PUBLIC FileAccess)
