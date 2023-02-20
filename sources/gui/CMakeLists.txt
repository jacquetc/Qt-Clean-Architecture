cmake_minimum_required(VERSION 3.5)

project(skribisto-gui VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools)

set(TS_FILES skribisto-gui_fr_FR.ts)

set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    ${TS_FILES}
)

qt_add_executable(skribisto-gui
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
)

# Define target properties for Android with Qt 6 as:
# set_property(TARGET skribisto-gui APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
# ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})

add_dependencies(${PROJECT_NAME} skribisto-contracts)
add_dependencies(${PROJECT_NAME} skribisto-adapters)
add_dependencies(${PROJECT_NAME} skribisto-persistence)
add_dependencies(${PROJECT_NAME} skribisto-domain)

target_link_libraries(skribisto-gui PRIVATE skribisto-contracts skribisto-adapters skribisto-persistence Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(skribisto-gui PUBLIC skribisto-domain)

include_directories("${CMAKE_SOURCE_DIR}/sources/contracts/")
include_directories("${CMAKE_SOURCE_DIR}/sources/adapters/")
include_directories("${CMAKE_SOURCE_DIR}/sources/persistence/")
include_directories("${CMAKE_SOURCE_DIR}/sources/domain/")

set_target_properties(skribisto-gui PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS skribisto-gui
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(skribisto-gui)
endif()
