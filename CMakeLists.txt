cmake_minimum_required(VERSION 3.16)

# specify the C compiler
set(CMAKE_C_COMPILER "gcc")

project(3dviewer LANGUAGES C CXX)

# specify the languages' standards
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
# specify the ouput directory
set(OUTPUT_BINARY_DIR ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_BINARY_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_BINARY_DIR}/lib)
# specify the output binaries' names
set(APP_NAME ${PROJECT_NAME})
set(LIB_NAME objviewer)

find_package(Qt6 REQUIRED COMPONENTS Widgets)
qt_standard_project_setup()

add_subdirectory(src/app)
add_subdirectory(src/lib)

target_link_libraries(${APP_NAME} PRIVATE
    ${LIB_NAME}
    Qt6::Widgets
)
