cmake_minimum_required(VERSION 3.10)
project(hello_package)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_library(greeting STATIC src/greeting.cpp)
target_include_directories(greeting PUBLIC include)

add_executable(hello_package src/main.cpp)
target_link_libraries(hello_package PRIVATE greeting)

enable_testing()

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.13.0.zip
)
FetchContent_MakeAvailable(googletest)

add_executable(test_package tests/tests.cpp)
target_link_libraries(test_package PRIVATE greeting gtest_main)
add_test(NAME GreetingTest COMMAND test_package)