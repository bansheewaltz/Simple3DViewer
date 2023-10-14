add_library(${LIB_NAME}
    src/affine_transformations/rotation.c
    src/affine_transformations/scaling.c
    src/affine_transformations/translation.c
    src/parsing/parsing.c
    include/obj_viewer.h
)

target_include_directories(${LIB_NAME} PUBLIC include)
