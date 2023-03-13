cmake_minimum_required(VERSION 3.16.0)

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -std=c++11  -pedantic -Wno-deprecated-declarations -fstrict-aliasing -pedantic-errors -Wno-unused-variable")
#
# Populate a CMake variable with the sources
set(CMAKE_INCLUDE_CURRENT_DIR ON)

project(skribisto-domain LANGUAGES CXX VERSION ${VERSION})

find_package(QT NAMES Qt6 COMPONENTS Core CONFIG REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core CONFIG REQUIRED)

set(domain_SRCS

    domain_global.h
author.h
entity.h
)

add_library(skribisto-domain SHARED ${domain_SRCS})
target_include_directories(skribisto-domain PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_definitions(skribisto-domain PRIVATE SKRIBISTO_DOMAIN_LIBRARY)
target_link_libraries(skribisto-domain PRIVATE Qt${QT_VERSION_MAJOR}::Core)


# ---------------------------------
if(IOS)
    install(TARGETS skribisto-domain LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
endif(IOS)

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    install(TARGETS skribisto-domain LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
endif(CMAKE_SYSTEM_NAME STREQUAL "Linux")

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    install(TARGETS skribisto-domain
        RUNTIME DESTINATION ${BINDIR}
        LIBRARY DESTINATION ${LIBDIR})
endif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
