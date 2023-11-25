find_package(Qt6 REQUIRED COMPONENTS OpenGLWidgets)

qt_standard_project_setup()

qt_add_executable(${PROJECT_NAME}
    main.cpp
    mainwindow.cpp mainwindow.hpp mainwindow.ui
    openglwidget.cpp openglwidget.hpp
    doubleslider.cpp doubleslider.hpp
)

target_include_directories(${PROJECT_NAME} PRIVATE .)

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::OpenGLWidgets
    QtGifImage
    libowviewer
)

set_target_properties(${APP_NAME} PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER smartcalc.bansheewaltz
    MACOSX_BUNDLE ON
    WIN32_EXECUTABLE ON
)
