cmake_minimum_required(VERSION 3.16.0)

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -std=c++11  -pedantic -Wno-deprecated-declarations -fstrict-aliasing -pedantic-errors -Wno-unused-variable")
#
# Populate a CMake variable with the sources
set(CMAKE_INCLUDE_CURRENT_DIR ON)

project(skribisto-infrastructure LANGUAGES CXX VERSION ${VERSION})

find_package(QT NAMES Qt6 COMPONENTS Core Concurrent Sql CONFIG REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Concurrent Sql CONFIG REQUIRED)

set(infrastructure_SRCS

    infrastructure_global.h
    infrastructure_registration.h infrastructure_registration.cpp
    skrib/skrib_loader.h skrib/skrib_loader.cpp
    skrib/skrib_file_configuration.h
)

add_library(skribisto-infrastructure SHARED ${infrastructure_SRCS})

target_compile_definitions(skribisto-infrastructure PRIVATE SKRIBISTO_INFRASTRUCTURE_LIBRARY)

add_dependencies(${PROJECT_NAME} skribisto-contracts)
#add_dependencies(${PROJECT_NAME} skribisto-domain)

target_link_libraries(skribisto-infrastructure PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Sql Qt${QT_VERSION_MAJOR}::Concurrent)
target_link_libraries(${PROJECT_NAME} PRIVATE skribisto-contracts)
#target_link_libraries(${PROJECT_NAME} PRIVATE skribisto-domain)

include_directories("${CMAKE_SOURCE_DIR}/sources/core/contracts/")
include_directories("${CMAKE_SOURCE_DIR}/sources/core/domain/")

# ---------------------------------
if(IOS)
    install(TARGETS skribisto-infrastructure LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
endif(IOS)

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    install(TARGETS skribisto-infrastructure LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
endif(CMAKE_SYSTEM_NAME STREQUAL "Linux")

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    install(TARGETS skribisto-infrastructure
        RUNTIME DESTINATION ${BINDIR}
        LIBRARY DESTINATION ${LIBDIR})
endif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
