cmake_minimum_required(VERSION 3.16)

set(CMAKE_C_COMPILER gcc)

project(3dviewer LANGUAGES C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(extern/gifimage)
add_subdirectory(src/lib)
add_subdirectory(src/app)
target_include_directories(${PROJECT_NAME} PRIVATE extern)
