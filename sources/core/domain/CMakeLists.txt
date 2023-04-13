# Populate a CMake variable with the sources
set(CMAKE_INCLUDE_CURRENT_DIR ON)

#set(PROJECT_NAME "example-domain")

project(example-domain LANGUAGES CXX VERSION ${VERSION})

find_package(QT NAMES Qt6 COMPONENTS Core CONFIG REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core CONFIG REQUIRED)

set(domain_SRCS

    domain_global.h
author.h
entity.h
)

add_library(example-domain SHARED ${domain_SRCS})
target_include_directories(example-domain PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_definitions(example-domain PUBLIC SKRIBISTO_DOMAIN_LIBRARY)
target_link_libraries(example-domain PRIVATE Qt${QT_VERSION_MAJOR}::Core)


# ---------------------------------
if(IOS)
    install(TARGETS example-domain LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
endif(IOS)

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    install(TARGETS example-domain LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
endif(CMAKE_SYSTEM_NAME STREQUAL "Linux")

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    install(TARGETS example-domain
        RUNTIME DESTINATION ${BINDIR}
        LIBRARY DESTINATION ${LIBDIR})
endif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
