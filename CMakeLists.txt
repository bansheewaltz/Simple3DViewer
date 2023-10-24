cmake_minimum_required(VERSION 3.16)

# specify the C compiler used to build the library
set(CMAKE_C_COMPILER "gcc")

project(3dviewer)

enable_language(CXX)
# specify the languages' standards
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
set(LIB_NAME libobjviewer)

find_package(Qt6 REQUIRED COMPONENTS OpenGLWidgets)
qt_standard_project_setup()

add_subdirectory(src/app)
add_subdirectory(src/lib)
# remove the prefix as it is already set
set_target_properties(${LIB_NAME} PROPERTIES PREFIX "")

target_link_libraries(${APP_NAME} PRIVATE
    ${LIB_NAME}
    Qt6::OpenGLWidgets
)
