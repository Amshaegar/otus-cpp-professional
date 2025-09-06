cmake_minimum_required(VERSION 3.12)

project(version_lib CXX)

add_library(version_lib
    lib.cpp
)

set_target_properties(version_lib PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

target_include_directories(version_lib
    PRIVATE "${CMAKE_BINARY_DIR}/version"
)

if (MSVC)
    target_compile_options(version_lib PUBLIC /W4 /WX)
else()
    target_compile_options(version_lib PUBLIC -Wall -Wextra -Werror -pedantic)
endif()