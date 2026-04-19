cmake_minimum_required(VERSION 3.10)
enable_testing()

set(LO3_SRC ${CMAKE_SOURCE_DIR}/src)
set(UNITY ${CMAKE_CURRENT_SOURCE_DIR}/unity/unity.c)
set(LO3_INCLUDES
    ${LO3_SRC}/internal
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/unity
)

# POSIX getline needs _POSIX_C_SOURCE
add_compile_definitions(_POSIX_C_SOURCE=200809L)

set(LO3_CORE_SOURCES
    ${LO3_SRC}/var.c
    ${LO3_SRC}/global.c
    ${LO3_SRC}/control-flow.c
    ${LO3_SRC}/execute.c
    ${LO3_SRC}/parsing.c
    ${LO3_SRC}/warnings.c
)

function(add_lo3_test name link_sources)
    add_executable(${name}
        ${CMAKE_CURRENT_SOURCE_DIR}/unit/${name}.c
        ${link_sources}
        ${UNITY}
    )
    target_include_directories(${name} PRIVATE ${LO3_INCLUDES})
    if(UNIX)
        target_link_libraries(${name} PRIVATE dl)
    endif()
    add_test(NAME ${name} COMMAND ${name})
endfunction()

# Each target links only what it needs
add_lo3_test(test_var
    "${LO3_SRC}/var.c;${LO3_SRC}/warnings.c")

add_lo3_test(test_global
    "${LO3_SRC}/global.c;${LO3_SRC}/warnings.c")

add_lo3_test(test_control_flow
    "${LO3_SRC}/control-flow.c;${LO3_SRC}/warnings.c")

add_lo3_test(test_warnings
    "${LO3_SRC}/warnings.c")

add_lo3_test(test_execute
    "${LO3_CORE_SOURCES}")

add_lo3_test(test_parsing
    "${LO3_CORE_SOURCES}")
