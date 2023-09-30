cmake_minimum_required (VERSION 3.25)

project (type_list)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_TEST_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -Wextra")

set(CMAKE_CXX_STANDARD 20)

add_library(type_list INTERFACE)
target_include_directories(type_list INTERFACE include)

if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    # We're in the root, define additional targets for developers.
    option(MY_PROJECT_BUILD_EXAMPLES   "whether or not examples should be built" ON)
    option(MY_PROJECT_BUILD_TESTS      "whether or not tests should be built" ON)

#    if(MY_PROJECT_BUILD_EXAMPLES)
#       add_subdirectory(examples)
#   endif()
    if(MY_PROJECT_BUILD_TESTS)
        enable_testing()
        add_subdirectory(tests)
    endif()
endif()
