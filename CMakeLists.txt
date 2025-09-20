cmake_minimum_required(VERSION 3.12)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VERSION 0.0.${PATCH_VERSION})

project(ip_filter VERSION ${PROJECT_VERSION})

configure_file(version/version.h.in version/version.h)

option(BUILD_TESTS "Whether to build tests" ON)

add_subdirectory(version)

if(BUILD_TESTS)
    add_subdirectory(tests)
endif()

add_executable(ip_filter
    "${CMAKE_CURRENT_SOURCE_DIR}/src/util.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/ip_filter.cpp"
    main.cpp
)

set_target_properties(ip_filter PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

target_link_libraries(ip_filter PRIVATE
    version_lib
)

target_include_directories(ip_filter
    PRIVATE "version"
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

if (MSVC)
    target_compile_options(ip_filter PUBLIC /W4 /WX)
else()
    target_compile_options(ip_filter PUBLIC -Wall -Wextra -Werror -pedantic)
endif()

install(TARGETS ip_filter RUNTIME DESTINATION bin)

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