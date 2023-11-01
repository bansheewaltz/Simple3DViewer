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

    src/affine_transformations/rotation.c
    src/affine_transformations/scaling.c
    src/affine_transformations/translation.c
    src/parsing/parsing.c
    src/utils.c
)

target_include_directories(${LIB_NAME} PUBLIC include)

add_executable(check src/main.c)
target_link_libraries(check ${LIB_NAME})
target_include_directories(check PUBLIC include)
target_include_directories(check PUBLIC extern)
#add_custom_command(
#    TARGET check
#    POST_BUILD
#    COMMAND check
#)
