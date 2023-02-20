cmake_minimum_required(VERSION 3.16.0)

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -std=c++11  -pedantic -Wno-deprecated-declarations -fstrict-aliasing -pedantic-errors -Wno-unused-variable")
#
# Populate a CMake variable with the sources
set(CMAKE_INCLUDE_CURRENT_DIR ON)

project(skribisto-persistence LANGUAGES CXX VERSION ${VERSION})

find_package(QT NAMES Qt6 COMPONENTS Core Concurrent Sql CONFIG REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Concurrent Sql CONFIG REQUIRED)

set(persistence_SRCS

    persistence_global.h
    persistence_registration.h persistence_registration.cpp
    repositories/generic_repository.h
    repositories/author_repository.h repositories/author_repository.cpp
    database/skrib/skrib_file.h
    database/skrib/skrib_file_context.h database/skrib/skrib_file_context.cpp
    repositories/repositories.h repositories/repositories.cpp
)

add_library(skribisto-persistence SHARED ${persistence_SRCS})

target_compile_definitions(skribisto-persistence PRIVATE SKRIBISTO_PERSISTENCE_LIBRARY)

add_dependencies(${PROJECT_NAME} skribisto-contracts)
add_dependencies(${PROJECT_NAME} skribisto-domain)

target_link_libraries(skribisto-persistence PRIVATE skribisto-domain skribisto-contracts Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Sql  Qt${QT_VERSION_MAJOR}::Concurrent)
#target_link_libraries(skribisto-persistence PRIVATE )

include_directories("${CMAKE_SOURCE_DIR}/sources/contracts/")
include_directories("${CMAKE_SOURCE_DIR}/sources/domain/")

# ---------------------------------
if(IOS)
    install(TARGETS skribisto-persistence LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
endif(IOS)

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    install(TARGETS skribisto-persistence LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
endif(CMAKE_SYSTEM_NAME STREQUAL "Linux")

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    install(TARGETS skribisto-persistence
        RUNTIME DESTINATION ${BINDIR}
        LIBRARY DESTINATION ${LIBDIR})
endif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
