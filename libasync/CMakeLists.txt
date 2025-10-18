cmake_minimum_required(VERSION 3.12)

project(libasync CXX)

add_library(${PROJECT_NAME} SHARED
    "${CMAKE_CURRENT_SOURCE_DIR}/Command.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Command.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/CoutOutputGenerator.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/CoutOutputGenerator.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/FileOutputGenerator.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/FileOutputGenerator.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Processor.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Processor.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/TimeProvider.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/TimeProvider.hpp"
)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD_REQUIRED ON)

target_include_directories(${PROJECT_NAME}
    PRIVATE "${CMAKE_BINARY_DIR}/libasync"
)

if (MSVC)
    target_compile_options(${PROJECT_NAME} PUBLIC /W4 /WX)
else()
    target_compile_options(${PROJECT_NAME} PUBLIC -Wall -Wextra -Werror -pedantic)
endif()
