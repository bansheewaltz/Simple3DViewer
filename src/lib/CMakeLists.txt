cmake_minimum_required(VERSION 3.16)

# set library name if it is not already specified from top-level
get_directory_property(IS_SUBPROJECT PARENT_DIRECTORY)

if(NOT IS_SUBPROJECT)
    set(LIB_NAME libobjviewer)
endif()

project(${LIB_NAME} LANGUAGES C)

# specify the languages' standards
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

add_library(${LIB_NAME} STATIC
    include/obj_viewer.h
    include/obj_viewer/details.h

    src/parsing.c
    src/processing.c

    src/rotation.c
    src/scale.c
    src/translation.c
    src/projection.c
)

target_include_directories(${LIB_NAME} PUBLIC include)
target_include_directories(${LIB_NAME} PUBLIC include/obj_viewer)

# Tests
add_executable(test_time test/time_test.cpp)
target_link_libraries(test_time ${LIB_NAME})
target_include_directories(test_time PRIVATE extern)
