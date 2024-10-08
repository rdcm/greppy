cmake_minimum_required(VERSION 3.14)
project(Greppy)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

set(THIRD_PARTY_DIR "${CMAKE_BINARY_DIR}")
file(MAKE_DIRECTORY ${THIRD_PARTY_DIR})

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        v1.15.2
  SOURCE_DIR     ${THIRD_PARTY_DIR}/googletest
)

FetchContent_MakeAvailable(googletest)

# application
add_executable(greppy
  src/main.cpp
  src/cpp/parser.cpp
  src/cpp/utils.cpp)

target_include_directories(greppy PRIVATE src/headers)

# tests
add_executable(tests 
  tests/parser_tests.cpp
  tests/utils_tests.cpp
  src/cpp/parser.cpp
  src/cpp/utils.cpp)

target_link_libraries(tests gtest_main)
include(GoogleTest)
gtest_discover_tests(tests)
