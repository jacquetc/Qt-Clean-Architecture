cmake_minimum_required(VERSION 3.5.0)

set(PROJECT_NAME "tst_skrib_database")

project(${PROJECT_NAME})

enable_testing()

# Tell CMake to run moc when necessary:
set(CMAKE_AUTOMOC ON)

# As moc files are generated in the binary dir, tell CMake
# to always look for includes there:
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Test Core Sql CONFIG REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Test Core Sql  REQUIRED)

set(QRC testfiles.qrc)
qt_add_resources(RESOURCES ${QRC})

add_executable(${PROJECT_NAME} ${PROJECT_NAME}.cpp ${RESOURCES})
add_test(${PROJECT_NAME} ${PROJECT_NAME})

add_dependencies(${PROJECT_NAME} example-contracts)
add_dependencies(${PROJECT_NAME} example-persistence)
add_dependencies(${PROJECT_NAME} example-domain)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Test Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Sql)

target_link_libraries(${PROJECT_NAME} PUBLIC example-contracts)
target_link_libraries(${PROJECT_NAME} PRIVATE example-persistence)
target_link_libraries(${PROJECT_NAME} PUBLIC example-domain)

include_directories("${CMAKE_SOURCE_DIR}/sources/core/contracts/")
include_directories("${CMAKE_SOURCE_DIR}/sources/core/persistence/")
include_directories("${CMAKE_SOURCE_DIR}/sources/core/domain/")
