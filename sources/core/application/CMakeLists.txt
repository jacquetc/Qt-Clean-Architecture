cmake_minimum_required(VERSION 3.16.0)

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -std=c++11  -pedantic -Wno-deprecated-declarations -fstrict-aliasing -pedantic-errors -Wno-unused-variable")
#
# Populate a CMake variable with the sources
set(CMAKE_INCLUDE_CURRENT_DIR ON)

project(skribisto-application LANGUAGES CXX VERSION ${VERSION})

find_package(QT NAMES Qt6 COMPONENTS Core Concurrent Sql CONFIG REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Concurrent Sql CONFIG REQUIRED)

set(application_SRCS

    application_global.h
    application_registration.h application_registration.cpp
    handler.h handler.cpp
    # author:
    features/author/handlers/queries/get_author_request_handler.h features/author/handlers/queries/get_author_request_handler.cpp
    features/author/handlers/commands/create_author_command_handler.h features/author/handlers/commands/create_author_command_handler.cpp
    features/author/handlers/commands/remove_author_command_handler.h features/author/handlers/commands/remove_author_command_handler.cpp
    features/author/handlers/commands/update_author_command_handler.h features/author/handlers/commands/update_author_command_handler.cpp
    features/author/handlers/queries/get_author_list_request_handler.h features/author/handlers/queries/get_author_list_request_handler.cpp
    # system:
    features/system/handlers/commands/save_system_as_command_handler.h features/system/handlers/commands/save_system_as_command_handler.cpp
    features/system/handlers/commands/load_system_command_handler.h features/system/handlers/commands/load_system_command_handler.cpp
)

add_library(skribisto-application SHARED ${application_SRCS})

add_dependencies(${PROJECT_NAME} skribisto-domain)
add_dependencies(${PROJECT_NAME} skribisto-contracts)

target_compile_definitions(skribisto-application PRIVATE SKRIBISTO_APPLICATION_LIBRARY)

target_link_libraries(skribisto-application PRIVATE skribisto-domain skribisto-contracts
     Qt6::Core Qt6::Sql Qt6::Concurrent)

include_directories("${CMAKE_SOURCE_DIR}/sources/core/domain/")
include_directories("${CMAKE_SOURCE_DIR}/sources/core/contracts/")

# ---------------------------------
if(IOS)
    install(TARGETS skribisto-application LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
endif(IOS)

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    install(TARGETS skribisto-application LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
endif(CMAKE_SYSTEM_NAME STREQUAL "Linux")

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    install(TARGETS skribisto-application
        RUNTIME DESTINATION ${BINDIR}
        LIBRARY DESTINATION ${LIBDIR})
endif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
