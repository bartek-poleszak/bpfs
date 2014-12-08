set(HEADER_FILES
    menuoptions.h
    main.h
)
set(SRC_LIST
    main.cpp
    prompt.cpp
    commandresolver.cpp
)

add_definitions(
    -std=c++11
    -Wall
)

#add_executable(manager main.cpp)
add_executable(manager ${SRC_LIST} ${HEADER_FILES})
target_link_libraries(manager LINK_PUBLIC core)
