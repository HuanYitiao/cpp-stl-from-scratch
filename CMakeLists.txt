cmake_minimum_required(VERSION 3.14)
project(ministl
    LANGUAGES CXX
    VERSION 0.1.0
    DESCRIPTION "A from-scratch implementation of C++ STL containers"
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Header-only interface library
add_library(ministl INTERFACE)
target_include_directories(ministl INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# Compiler warnings
target_compile_options(ministl INTERFACE
    $<$<CXX_COMPILER_ID:GNU>:-Wall -Wextra -Wpedantic -Werror>
    $<$<CXX_COMPILER_ID:Clang>:-Wall -Wextra -Wpedantic -Werror>
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
)

# Tests
option(MINISTL_BUILD_TESTS "Build unit tests" ON)
if(MINISTL_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
