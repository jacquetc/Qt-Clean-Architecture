# Populate a CMake variable with the sources
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(PROJECT_NAME "example-persistence")

project(example-persistence LANGUAGES CXX VERSION ${VERSION})

find_package(QT NAMES Qt6 COMPONENTS Core Concurrent Sql CONFIG REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Concurrent Sql CONFIG REQUIRED)

set(persistence_SRCS

    persistence_global.h
    persistence_registration.h persistence_registration.cpp
    repositories/generic_repository.h
    repositories/author_repository.h repositories/author_repository.cpp
    database/internal_database.h
    database/database_context.h database/database_context.cpp
    repositories/repository_provider.h repositories/repository_provider.cpp
)

add_library(example-persistence SHARED ${persistence_SRCS})
target_include_directories(example-persistence PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

target_compile_definitions(example-persistence PUBLIC SKRIBISTO_PERSISTENCE_LIBRARY)

add_dependencies(example-persistence example-contracts)

target_link_libraries(example-persistence PUBLIC example-contracts)
target_link_libraries(example-persistence PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Sql Qt${QT_VERSION_MAJOR}::Concurrent)

# ---------------------------------
if(IOS)
    install(TARGETS example-persistence LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
endif(IOS)

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    install(TARGETS example-persistence LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
endif(CMAKE_SYSTEM_NAME STREQUAL "Linux")

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    install(TARGETS example-persistence
        RUNTIME DESTINATION ${BINDIR}
        LIBRARY DESTINATION ${LIBDIR})
endif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
