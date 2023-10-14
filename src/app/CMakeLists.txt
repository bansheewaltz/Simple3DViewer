qt_add_executable(${APP_NAME}
    mainwindow.ui
    mainwindow.cpp
    main.cpp
)

set_target_properties(${APP_NAME} PROPERTIES
    WIN32_EXECUTABLE ON
    MACOSX_BUNDLE ON
)
