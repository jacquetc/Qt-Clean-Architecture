cmake_minimum_required(VERSION 3.16.0)

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -std=c++11  -pedantic -Wno-deprecated-declarations -fstrict-aliasing -pedantic-errors -Wno-unused-variable")
#
# Populate a CMake variable with the sources
set(CMAKE_INCLUDE_CURRENT_DIR ON)

project(skribisto-presenter LANGUAGES CXX VERSION ${VERSION})

find_package(QT NAMES Qt6 COMPONENTS Core Gui CONFIG REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Gui CONFIG REQUIRED)

set(presenter_SRCS

    presenter_global.h
    presenter_registration.h presenter_registration.cpp
    author/author_controller.h
    author/author_undo_commands.h author/author_undo_commands.cpp
    undo_controller.h undo_controller.cpp
    system/system_controller.h
)

add_library(skribisto-presenter SHARED ${presenter_SRCS})

add_dependencies(${PROJECT_NAME} skribisto-domain)
add_dependencies(${PROJECT_NAME} skribisto-contracts)
add_dependencies(${PROJECT_NAME} skribisto-application)
add_dependencies(${PROJECT_NAME} skribisto-persistence)

target_compile_definitions(skribisto-presenter PRIVATE SKRIBISTO_PRESENTER_LIBRARY)

target_link_libraries(skribisto-presenter PRIVATE
    skribisto-domain
    skribisto-contracts
    skribisto-application
    skribisto-persistence
    Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Gui)

include_directories("${CMAKE_SOURCE_DIR}/sources/application/")
include_directories("${CMAKE_SOURCE_DIR}/sources/persistence/")
include_directories("${CMAKE_SOURCE_DIR}/sources/contracts/")
include_directories("${CMAKE_SOURCE_DIR}/sources/domain/")

# ---------------------------------
if(IOS)
    install(TARGETS skribisto-presenter LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
endif(IOS)

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    install(TARGETS skribisto-presenter LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
endif(CMAKE_SYSTEM_NAME STREQUAL "Linux")

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    install(TARGETS skribisto-presenter
        RUNTIME DESTINATION ${BINDIR}
        LIBRARY DESTINATION ${LIBDIR})
endif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
