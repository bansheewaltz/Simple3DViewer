cmake_minimum_required(VERSION 3.16)

project(libowviewer LANGUAGES C)
get_directory_property(IS_SUBPROJECT PARENT_DIRECTORY)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_STATIC_LIBRARY_PREFIX "")

add_library(${PROJECT_NAME} STATIC
    include/owviewer.h
    include/owviewer/details.h

    src/parsing.c
    src/processing.c

    src/rotation.c
    src/scale.c
    src/translation.c
    src/projection.c
)

target_include_directories(${PROJECT_NAME} PUBLIC include include/owviewer)

add_executable(test_time test/tests/time_test.cpp)
target_link_libraries(test_time ${PROJECT_NAME})
target_include_directories(test_time PRIVATE test/3rdparty)
