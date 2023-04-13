cmake_minimum_required(VERSION 3.5.0)

set(PROJECT_NAME "tst_usecases")

project(${PROJECT_NAME})

enable_testing()

# Tell CMake to run moc when necessary:
set(CMAKE_AUTOMOC ON)

# As moc files are generated in the binary dir, tell CMake
# to always look for includes there:
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Test Core CONFIG REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Test Core REQUIRED)

#set(QRC ${CMAKE_SOURCE_DIR}/resources/test/testfiles.qrc)
#qt_add_resources(RESOURCES ${QRC})

add_executable(${PROJECT_NAME} ${PROJECT_NAME}.cpp ${RESOURCES}
    dummy_author_repository.h)
add_test(${PROJECT_NAME} ${PROJECT_NAME})

add_dependencies(${PROJECT_NAME} example-contracts)
add_dependencies(${PROJECT_NAME} example-application)
add_dependencies(${PROJECT_NAME} example-domain)
add_dependencies(${PROJECT_NAME} example-infrastructure)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Test Qt${QT_VERSION_MAJOR}::Core )

target_link_libraries(${PROJECT_NAME} PUBLIC example-contracts)
target_link_libraries(${PROJECT_NAME} PUBLIC example-application)
target_link_libraries(${PROJECT_NAME} PUBLIC example-domain)
target_link_libraries(${PROJECT_NAME} PRIVATE example-infrastructure)

include_directories("${CMAKE_SOURCE_DIR}/sources/core/contracts/")
include_directories("${CMAKE_SOURCE_DIR}/sources/core/application/")
include_directories("${CMAKE_SOURCE_DIR}/sources/core/domain/")
include_directories("${CMAKE_SOURCE_DIR}/sources/core/infrastructure/")
