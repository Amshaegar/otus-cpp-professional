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
    "${CMAKE_CURRENT_SOURCE_DIR}/../src/util.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/../src/ip_filter.cpp"
    tests.cpp
    main.cpp
)

set_target_properties(tests PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

target_include_directories(
    tests PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/../version"
    "${CMAKE_CURRENT_SOURCE_DIR}/../src"
)

target_link_libraries(tests PRIVATE gtest)
target_link_libraries(tests PUBLIC version_lib)

if (MSVC)
    target_compile_options(tests PUBLIC /W4 /WX)
else()
    target_compile_options(tests PUBLIC -Wall -Wextra -Werror -pedantic)
endif()