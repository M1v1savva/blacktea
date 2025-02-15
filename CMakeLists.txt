cmake_minimum_required(VERSION 3.10)
project(PackageManager)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(CURL REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(glog REQUIRED)

include_directories(${CURL_INCLUDE_DIRS} ${OPENSSL_INCLUDE_DIR} ${GLOG_INCLUDE_DIRS})

set(SOURCES
    src/package_manager.cpp
)

add_executable(package_manager ${SOURCES})

target_link_libraries(package_manager
    ${CURL_LIBRARIES}
    OpenSSL::SSL
    OpenSSL::Crypto
    nlohmann_json::nlohmann_json
    glog::glog
)

install(TARGETS package_manager DESTINATION bin)