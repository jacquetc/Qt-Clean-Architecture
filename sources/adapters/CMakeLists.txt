cmake_minimum_required(VERSION 3.16.0)

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -std=c++11  -pedantic -Wno-deprecated-declarations -fstrict-aliasing -pedantic-errors -Wno-unused-variable")
#
# Populate a CMake variable with the sources
set(CMAKE_INCLUDE_CURRENT_DIR ON)

project(skribisto-adapters LANGUAGES CXX VERSION ${VERSION})

find_package(QT NAMES Qt6 COMPONENTS Core CONFIG REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core CONFIG REQUIRED)

set(adapters_SRCS

    adapters_global.h
)

add_library(skribisto-adapters SHARED ${adapters_SRCS})

add_dependencies(${PROJECT_NAME} skribisto-application)
add_dependencies(${PROJECT_NAME} skribisto-contracts)

target_compile_definitions(skribisto-adapters PRIVATE SKRIBISTO_ADAPTERS_LIBRARY)

target_link_libraries(skribisto-adapters PRIVATE skribisto-application skribisto-contracts Qt${QT_VERSION_MAJOR}::Core)

include_directories("${CMAKE_SOURCE_DIR}/sources/application/")
include_directories("${CMAKE_SOURCE_DIR}/sources/contracts/")

# ---------------------------------
if(IOS)
    install(TARGETS skribisto-adapters LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
endif(IOS)

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    install(TARGETS skribisto-adapters LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
endif(CMAKE_SYSTEM_NAME STREQUAL "Linux")

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    install(TARGETS skribisto-adapters
        RUNTIME DESTINATION ${BINDIR}
        LIBRARY DESTINATION ${LIBDIR})
endif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
