cmake_minimum_required(VERSION 3.10)

file(STRINGS "version.txt" LO3_VERSION)
project(lo3 VERSION ${LO3_VERSION} C)
set(CMAKE_C_STANDARD 11)

configure_file(include/version.h.in ${CMAKE_BINARY_DIR}/version.h)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/bin)

file(GLOB SOURCES "src/*.c")
add_executable(lo3 ${SOURCES})
target_include_directories(lo3 PRIVATE include ${CMAKE_BINARY_DIR})

if(UNIX)
    target_link_libraries(lo3 PRIVATE dl)
endif()

enable_testing()
add_subdirectory(test)