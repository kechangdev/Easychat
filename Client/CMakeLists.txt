cmake_minimum_required(VERSION 3.5)

project(Easychat VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 查找Qt包，包含Widgets和Network模块
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Network)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Network)

set(PROJECT_SOURCES
        main.cpp
        login_window.cpp
        login_window.h
        login_window.ui
        chatwindow.cpp
        chatwindow.h
        chatwindow.ui
        signupwindow.cpp
        signupwindow.h
        signupwindow.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Easychat
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        signupwindow.h signupwindow.cpp signupwindow.ui
        chatwindow.h chatwindow.cpp chatwindow.ui
        userlist.h userlist.cpp userlist.ui
        notification.h notification.cpp notification.ui

    )
else()
    if(ANDROID)
        add_library(Easychat SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(Easychat
            ${PROJECT_SOURCES}
        )
    endif()
endif()

# 链接Qt库，包含Widgets和Network模块
target_link_libraries(Easychat PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Network)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.Easychat)
endif()
set_target_properties(Easychat PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS Easychat
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Easychat)
endif()
