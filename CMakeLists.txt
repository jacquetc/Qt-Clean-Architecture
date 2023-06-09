cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -std=c++11  -pedantic -Wno-deprecated-declarations -fstrict-aliasing -pedantic-errors -Wno-unused-variable")
#

# forbid some old things
add_definitions(-DQT_NO_FOREACH)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(QT_MIN_VERSION "6.4.0")
set(QT_DEFAULT_MAJOR_VERSION 6)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/ ${CMAKE_SOURCE_DIR}/cmake/3rdparty/ ${CMAKE_MODULE_PATH})

set(VERSION 0.2)

project(example VERSION ${VERSION} LANGUAGES CXX)

include(CMakePackageConfigHelpers)

find_package(Qt6 REQUIRED COMPONENTS Core)
qt_standard_project_setup()
set(CMAKE_AUTORCC ON)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(SKR_DEBUG true)
    add_compile_definitions(SKR_DEBUG=true)
    add_subdirectory(tests)

else()
    set(SKR_DEBUG false)
    add_compile_definitions(SKR_DEBUG=false)
endif(CMAKE_BUILD_TYPE STREQUAL "Debug")

add_subdirectory(sources/core/domain)
add_subdirectory(sources/core/contracts)
add_subdirectory(sources/core/persistence)
add_subdirectory(sources/core/infrastructure)
add_subdirectory(sources/core/application)
add_subdirectory(sources/gui/presenter)
add_subdirectory(sources/gui/desktop)

# generate documentation
find_program(QDOC_EXECUTABLE qdoc)
message("doc : ${QDOC_EXECUTABLE}")
if(QDOC_EXECUTABLE)
  # Set the output directory for the generated documentation
  set(DOCS_OUTPUT_DIR "${CMAKE_SOURCE_DIR}/docs")

  # Add a custom command to clean the output directory
  add_custom_command(
    OUTPUT clean_docs_output
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${DOCS_OUTPUT_DIR}
    COMMAND ${CMAKE_COMMAND} -E make_directory ${DOCS_OUTPUT_DIR}
    COMMENT "Cleaning documentation output directory"
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  )

  # Add a custom target to generate documentation with QDoc
  add_custom_target(docs
    COMMAND ${QDOC_EXECUTABLE} ${CMAKE_SOURCE_DIR}/example.qdocconf
    DEPENDS clean_docs_output
    COMMENT "Generating documentation with QDoc"
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  )

  # Add the following line to make the 'docs' target a dependency of the 'all' target
  # add_dependencies(example-domain docs)
endif()

