cmake_minimum_required(VERSION 3.16)
project(IPOverlay VERSION 0.1 LANGUAGES CXX)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 增加 Network
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Network LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Network LinguistTools)

set(TS_FILES IPOverlay_zh_CN.ts)
set(PROJECT_SOURCES
        main.cpp
        floatingipoverlaywidget.cpp
        floatingipoverlaywidget.h
        floatingipoverlaywidget.ui
        ${TS_FILES}
)
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(IPOverlay
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        ipservice.h
        ipservice.cpp
        Widget.h
        Widget.cpp
        AutoStartHelper.h
        AutoStartHelper.cpp
        traymanager.h
        traymanager.cpp
        ConfigHelper.h
        ConfigHelper.cpp
        SettingsDialog.h
        SettingsDialog.cpp
        SettingsDialog.ui
        aboutdialog.h aboutdialog.cpp aboutdialog.ui
        IPOverlay.qrc
    )
    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    if(ANDROID)
        add_library(IPOverlay SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(IPOverlay
            ${PROJECT_SOURCES}
        )
    endif()
    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

# 这里加上 Network
target_link_libraries(IPOverlay PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Network
)

if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.IPOverlay)
endif()
set_target_properties(IPOverlay PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)
include(GNUInstallDirs)
install(TARGETS IPOverlay
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(IPOverlay)
endif()
