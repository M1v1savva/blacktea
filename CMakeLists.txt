cmake_minimum_required(VERSION 3.10)
project(package_manager)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include(FetchContent)

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

set(WITH_GFLAGS OFF CACHE BOOL "Disable gflags in glog" FORCE)
set(BUILD_TESTING OFF CACHE BOOL "Disable glog tests" FORCE)
set(GLOG_BUILD_TESTS OFF CACHE BOOL "Disable glog tests" FORCE)
set(GLOG_INSTALL OFF CACHE BOOL "Prevent glog installation" FORCE)
set(GLOG_BENCHMARK OFF CACHE BOOL "Disable glog benchmarks" FORCE)

FetchContent_Declare(
  glog
  URL https://github.com/google/glog/archive/refs/tags/v0.6.0.zip
)
FetchContent_MakeAvailable(glog)

FetchContent_Declare(
  curl
  URL https://github.com/curl/curl/releases/download/curl-8_1_2/curl-8.1.2.tar.gz
)

FetchContent_GetProperties(curl)
if(NOT curl_POPULATED)
  FetchContent_Populate(curl)
  set(BUILD_CURL_EXE "OFF" CACHE BOOL "Disable building curl CLI" FORCE)
  set(BUILD_SHARED_LIBS "ON" CACHE BOOL "Build shared libraries" FORCE)
  add_subdirectory(${curl_SOURCE_DIR} ${curl_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

find_package(OpenSSL REQUIRED)

add_library(core STATIC
    src/package_manager/download.cpp
    src/package_manager/utils.cpp
    src/package_manager/fetch_package_list.cpp
    src/package_manager/CurledData.cpp
    src/package_manager/Metadata.cpp
)

if(NOT EXISTS ${PROJECT_SOURCE_DIR}/src/package_manager/download.cpp OR
   NOT EXISTS ${PROJECT_SOURCE_DIR}/src/package_manager/utils.cpp OR
   NOT EXISTS ${PROJECT_SOURCE_DIR}/src/package_manager/fetch_package_list.cpp OR
   NOT EXISTS ${PROJECT_SOURCE_DIR}/src/package_manager/CurledData.cpp OR
   NOT EXISTS ${PROJECT_SOURCE_DIR}/src/package_manager/Metadata.cpp)
  message(FATAL_ERROR "One or more source files for 'core' library are missing.")
endif()

target_include_directories(core
  PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${nlohmann_json_SOURCE_DIR}/include
    ${glog_SOURCE_DIR}/src
    ${curl_SOURCE_DIR}/include
    ${curl_BINARY_DIR}
    ${OPENSSL_INCLUDE_DIR}
)

target_link_libraries(core
  PRIVATE
    libcurl
    OpenSSL::SSL
    OpenSSL::Crypto
    nlohmann_json::nlohmann_json
    glog::glog
)

add_executable(package_manager src/main.cpp)
target_include_directories(package_manager PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(package_manager PRIVATE core)

enable_testing()

add_executable(tests tests/tests.cpp)
target_include_directories(tests PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(tests PRIVATE gtest_main core)
add_test(NAME PackageManagerTest COMMAND tests)

set_tests_properties(PackageManagerTest PROPERTIES LABELS "unit")

install(TARGETS package_manager DESTINATION bin)
