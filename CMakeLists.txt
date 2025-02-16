cmake_minimum_required(VERSION 3.10)
project(package_manager)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include(FetchContent)

# Dependencies
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.13.0.zip
)
FetchContent_MakeAvailable(googletest)

FetchContent_Declare(
  nlohmann_json
  URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz
)
FetchContent_MakeAvailable(nlohmann_json)

FetchContent_Declare(
  glog
  URL https://github.com/google/glog/archive/refs/tags/v0.6.0.zip
)
FetchContent_MakeAvailable(glog)

# Fetch and build curl
FetchContent_Declare(
  curl
  URL https://github.com/curl/curl/releases/download/curl-8_1_2/curl-8.1.2.tar.gz
)
FetchContent_GetProperties(curl)
if(NOT curl_POPULATED)
  FetchContent_Populate(curl)
  # Force static build for curl:
  set(CURL_STATICLIB TRUE CACHE BOOL "Force static build for Curl")
  set(BUILD_SHARED_LIBS OFF CACHE BOOL "Disable shared Curl library" FORCE)
  set(BUILD_CURL_EXE OFF CACHE BOOL "Disable building curl CLI" FORCE)
  # Use a unique build directory for curl:
  set(CURL_CUSTOM_BINARY_DIR "${CMAKE_BINARY_DIR}/curl_build")
  add_subdirectory(${curl_SOURCE_DIR} ${CURL_CUSTOM_BINARY_DIR})
endif()

# Core library
add_library(core STATIC
    src/package_manager/download.cpp
    src/package_manager/utils.cpp
    src/package_manager/fetch_package_list.cpp
    src/package_manager/CurledData.cpp
    src/package_manager/Metadata.cpp
)
target_include_directories(core PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${nlohmann_json_SOURCE_DIR}/include
    ${curl_SOURCE_DIR}/include
)

target_link_libraries(core PRIVATE
    libcurl
    nlohmann_json::nlohmann_json
    glog::glog
)

# Main executable
add_executable(package_manager src/main.cpp)
target_include_directories(package_manager PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(package_manager PRIVATE core libcurl)

# Tests
enable_testing()
add_executable(tests tests/tests.cpp)
target_include_directories(tests PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${curl_SOURCE_DIR}/include
    ${nlohmann_json_SOURCE_DIR}/include
)
target_link_libraries(tests PRIVATE gtest_main core libcurl glog::glog)
add_test(NAME PackageManagerTest COMMAND tests)

install(TARGETS package_manager DESTINATION bin)
