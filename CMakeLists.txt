cmake_minimum_required(VERSION 3.12)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VERSION 0.0.${PATCH_VERSION})
set(PROJECT_NAME "bulk")
project(${PROJECT_NAME} VERSION ${PROJECT_VERSION})

configure_file(version/version.h.in version/version.h)

option(BUILD_TESTS "Whether to build tests" ON)

add_subdirectory(version)

if(BUILD_TESTS)
    add_subdirectory(tests)
endif()

add_executable(${PROJECT_NAME}
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Command.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Command.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/CoutOutputGenerator.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/CoutOutputGenerator.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/FileOutputGenerator.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/FileOutputGenerator.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Processor.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Processor.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/TimeProvider.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/TimeProvider.hpp"
    main.cpp
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    version_lib
)

target_include_directories(${PROJECT_NAME}
    PRIVATE "version"
    PRIVATE "src"
)

if (MSVC)
    target_compile_options(${PROJECT_NAME} PUBLIC /W4 /WX)
else()
    target_compile_options(${PROJECT_NAME} PUBLIC -Wall -Wextra -Werror -pedantic)
endif()

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT example@example.com)
include(CPack)

if(BUILD_TESTS)
    enable_testing()
    add_test(tests tests/tests)
endif()
