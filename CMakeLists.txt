cmake_minimum_required(VERSION 3.5)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)

# forbid some old things
add_definitions(-DQT_NO_FOREACH)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(QT_MIN_VERSION "6.2.2")
set(QT_DEFAULT_MAJOR_VERSION 6)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/ ${CMAKE_SOURCE_DIR}/cmake/3rdparty/ ${CMAKE_MODULE_PATH})

set(VERSION 0.1)

project(skribisto VERSION ${VERSION} LANGUAGES CXX)

include(CMakePackageConfigHelpers)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(SKR_DEBUG true)
    add_compile_definitions(SKR_DEBUG=true)
   # add_subdirectory(tests)

else()
    set(SKR_DEBUG false)
    add_compile_definitions(SKR_DEBUG=false)
endif(CMAKE_BUILD_TYPE STREQUAL "Debug")

add_subdirectory(sources/core/domain)
add_subdirectory(sources/core/contracts)
add_subdirectory(sources/core/persistence)
#add_subdirectory(sources/core/infrastructure)
#add_subdirectory(sources/core/application)
#add_subdirectory(sources/gui/presenter)
#add_subdirectory(sources/gui/desktop)
