#include "package_manager.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <curl/curl.h>
#include <glog/logging.h>
#include <nlohmann/json.hpp>
#include <cstdlib>

namespace package_manager {

const std::string kRepoUrl = "https://github.com/M1v1savva/blacktea_package_manager";
const std::string kPackageListUrl = kRepoUrl + "/archive/refs/tags/package_list_v0.1.0.tar.gz";
const std::string kPackageDir = "packages";
const std::string kSelfPackageName = 'package_manager_v0.1.0';
const std::string kRepoListPath = kPackageDir + "/repo_list.json"
const std::string kEnvListPath = kPackageDir + "/env_list.json";

size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    std::ofstream* out = static_cast<std::ofstream*>(userp);
    out->write(static_cast<char*>(contents), size * nmemb);
    return size * nmemb;
}

bool fetch_package_metadata() {
    if (!std::filesystem::exists(kPackageDir)) {
        std::filesystem::create_directories(kPackageDir);
    }
    
    CURL* curl = curl_easy_init();
    if (!curl) {
        LOG(ERROR) << "Failed to initialize curl.";
        return false;
    }

    std::ofstream out_file(kRepoListPath);
    if (!out_file.is_open()) {
        LOG(ERROR) << "Failed to create package list file.";
        return false;
    }

    curl_easy_setopt(curl, CURLOPT_URL, kPackageListUrl.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &out_file);
    CURLcode res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    out_file.close();

    if (res != CURLE_OK) {
        LOG(ERROR) << "Failed to fetch package list: " << curl_easy_strerror(res);
        return false;
    }

    LOG(INFO) << "Package list updated successfully.";
    return true;
}

bool download_package(const std::string& package_name) {
	std::ifstream metadata_file(kRepoListPath);
    if (!metadata_file) {
        LOG(ERROR) << "Package list not found at " << kRepoListPath;
        return false;
    }

	nlohmann::json metadata;
    try {
        metadata_file >> metadata;
    } catch (const std::exception& e) {
        LOG(ERROR) << "Failed to parse JSON package list: " << e.what();
        return false;
    }

    if (!metadata.contains(package_name)) {
        LOG(ERROR) << "Package name not found in package list: " << package_name;
        return false;
    }

    if (!metadata[package_name].contains("url") || metadata[package_name]["url"].is_null()) {
        LOG(ERROR) << "No valid URL found for package: " << package_name;
        return false;
    }

    std::string package_url = metadata[package_name]["url"];
    std::string package_path = kPackageDir + "/" + package_name + ".tar.gz";

	CURL* curl = curl_easy_init();
    if (!curl) {
        LOG(ERROR) << "Failed to initialize curl";
        return false;
    }

	std::ofstream out_file(package_path, std::ios::binary);
    if (!out_file.is_open()) {
        LOG(ERROR) << "Failed to create package file at: " << package_path;
        curl_easy_cleanup(curl);
        return false;
    }

	curl_easy_setopt(curl, CURLOPT_URL, package_url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &out_file);
    CURLcode res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    out_file.close();

	if (res != CURLE_OK) {
        LOG(ERROR) << "Download failed: " << curl_easy_strerror(res);
        return false;
    }

    LOG(INFO) << "Download complete: " << package_path;
    return true;
}

bool install_package(const std::string& package_name) {
    
}

bool self_update() {

}

bool verify_package_hash(const std::string& package_name) {

}

} 