set(SOURCES
    ./src/affine_transformations/rotation.c
    ./src/affine_transformations/scaling.c
    ./src/affine_transformations/translation.c
    ./src/parsing/parsing.c
    ./include/obj_viewer.h
)
add_library(ObjViewer ${SOURCES})
target_include_directories(ObjViewer PUBLIC include)
