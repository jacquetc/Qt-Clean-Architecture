cmake_minimum_required(VERSION 3.16.0)

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -std=c++11  -pedantic -Wno-deprecated-declarations -fstrict-aliasing -pedantic-errors -Wno-unused-variable")
#
# Populate a CMake variable with the sources
set(CMAKE_INCLUDE_CURRENT_DIR ON)

project(skribisto-contracts LANGUAGES CXX VERSION ${VERSION})

find_package(QT NAMES Qt6 COMPONENTS Core CONFIG REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core CONFIG REQUIRED)

set(contracts_SRCS

    contracts_global.h
    exception.h
    database/interface_database.h
    database/interface_database_context.h
    automapper/automapper.h
    error.h
    wait_in_event_loop.h
    result.h
    # persistence:
    persistence/interface_generic_repository.h
    persistence/interface_author_repository.h
    persistence/interface_repository.h
    persistence/interface_repository_provider.h
    # author:
    dto/author/author_dto.h
    dto/author/create_author_dto.h
    dto/author/author_dto_base.h
    dto/author/update_author_dto.h
    cqrs/author/requests/get_author_request.h
    cqrs/author/commands/create_author_command.h
    cqrs/author/commands/remove_author_command.h
    cqrs/author/commands/update_author_command.h
    cqrs/author/validators/create_author_command_validator.h
    cqrs/author/validators/update_author_command_validator.h
    # system:
    dto/system/save_system_as_dto.h
    dto/system/load_system_dto.h
    cqrs/system/commands/save_system_as_command.h
    cqrs/system/commands/load_system_command.h
    cqrs/system/validators/save_system_as_command_validator.h
    cqrs/system/validators/load_system_command_validator.h

)

add_library(skribisto-contracts SHARED ${contracts_SRCS})

target_compile_definitions(skribisto-contracts PRIVATE SKRIBISTO_CONTRACTS_LIBRARY)

add_dependencies(${PROJECT_NAME} skribisto-domain)

target_link_libraries(skribisto-contracts PRIVATE skribisto-domain Qt${QT_VERSION_MAJOR}::Core)
include_directories("${CMAKE_SOURCE_DIR}/sources/core/domain/")

# ---------------------------------
if(IOS)
    install(TARGETS skribisto-contracts LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
endif(IOS)

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    install(TARGETS skribisto-contracts LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
endif(CMAKE_SYSTEM_NAME STREQUAL "Linux")

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    install(TARGETS skribisto-contracts
        RUNTIME DESTINATION ${BINDIR}
        LIBRARY DESTINATION ${LIBDIR})
endif(CMAKE_SYSTEM_NAME STREQUAL "Windows")