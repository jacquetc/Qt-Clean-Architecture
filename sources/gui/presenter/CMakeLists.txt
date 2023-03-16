cmake_minimum_required(VERSION 3.16.0)

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -std=c++11  -pedantic -Wno-deprecated-declarations -fstrict-aliasing -pedantic-errors -Wno-unused-variable")
#
# Populate a CMake variable with the sources
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(PROJECT_NAME "skribisto-presenter")

project(${PROJECT_NAME} LANGUAGES CXX VERSION ${VERSION})

find_package(QT NAMES Qt6 COMPONENTS Core Gui Concurrent CONFIG REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Gui Concurrent CONFIG REQUIRED)

set(presenter_SRCS

    presenter_global.h
    presenter_registration.h presenter_registration.cpp
    # author
    author/author_controller.h author/author_controller.cpp
    author/author_undo_commands.h author/author_undo_commands.cpp
    author/author_signal_bridge.h
    # system
    system/system_controller.h
    system/system_controller.cpp
    system/system_undo_command.h system/system_undo_command.cpp
    system/system_signal_bridge.h
    # undo redo
    undo_redo/undo_redo_command.h undo_redo/undo_redo_command.cpp
    undo_redo/undo_redo_system.h undo_redo/undo_redo_system.cpp
    undo_redo/threaded_undo_redo_system.h undo_redo/threaded_undo_redo_system.cpp
    undo_redo/query_command.h undo_redo/query_command.cpp
)

add_library(${PROJECT_NAME} STATIC ${presenter_SRCS})
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

target_compile_definitions(${PROJECT_NAME} PUBLIC SKRIBISTO_PRESENTER_LIBRARY)


add_dependencies(${PROJECT_NAME} skribisto-contracts)
add_dependencies(${PROJECT_NAME} skribisto-application)
add_dependencies(${PROJECT_NAME} skribisto-persistence)
add_dependencies(${PROJECT_NAME} skribisto-infrastructure)

target_link_libraries(${PROJECT_NAME} PUBLIC skribisto-application)
target_link_libraries(${PROJECT_NAME} PUBLIC skribisto-persistence)
target_link_libraries(${PROJECT_NAME} PUBLIC skribisto-contracts)
target_link_libraries(${PROJECT_NAME} PUBLIC skribisto-infrastructure)

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Gui Qt${QT_VERSION_MAJOR}::Concurrent)

# ---------------------------------
if(IOS)
    install(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
endif(IOS)

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    install(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
endif(CMAKE_SYSTEM_NAME STREQUAL "Linux")

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION ${BINDIR}
        LIBRARY DESTINATION ${LIBDIR})
endif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
