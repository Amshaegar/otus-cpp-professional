cmake_minimum_required(VERSION 3.12)

project(tests CXX)

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        v1.16.0
)
FetchContent_MakeAvailable(googletest)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

add_executable(tests
    "${CMAKE_CURRENT_SOURCE_DIR}/../src/Command.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/../src/Command.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/../src/CoutOutputGenerator.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/../src/CoutOutputGenerator.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/../src/FileOutputGenerator.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/../src/FileOutputGenerator.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/../src/Processor.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/../src/Processor.hpp"
    main.cpp
    tests.cpp
)

set_target_properties(tests PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

target_include_directories(
    tests PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/../src"
    "${CMAKE_CURRENT_SOURCE_DIR}/../version"
)

target_link_libraries(tests PRIVATE gtest)
target_link_libraries(tests PUBLIC version_lib)

if (MSVC)
    target_compile_options(tests PUBLIC /W4 /WX)
else()
    target_compile_options(tests PUBLIC -Wall -Wextra -Werror -pedantic)
endif()
