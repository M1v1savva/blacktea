#include "package_manager/package_manager.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <curl/curl.h>
#include <glog/logging.h>
#include <nlohmann/json.hpp>
#include <cstdlib>

namespace package_manager {

const std::string kRepoUrl = "https://github.com/M1v1savva/blacktea";
const std::string kPackageListUrl = kRepoUrl + "/releases/download/package_list_v0.1/package_list.json";
const std::string kSelfName = "blacktea_package_manager_v0.1";
const std::string kSelfShortName = "blacktea_package_manager";

const std::string kPackageDir = "../../packages";
const std::string kPackageListPath = kPackageDir + "/package_list.json";
const std::string kEnvListPath = kPackageDir + "/env_list.json";

size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
    size_t total_size = size * nmemb;
    output->append((char*)contents, total_size);
    return total_size;
}

// TODO: rewrite install, sha256 part
// TODO: logs in a file
// TODO: self-update, run, delete
// TODO: write tests and CMakeLists
// TODO: write scripts
// TODO: go through hello_world package once more
// TODO: each package has ./install.sh
// TODO: each script is run from /build with bash <script_name>.sh

bool fetch_package_list() {
    if (!std::filesystem::exists(kPackageDir)) {
        std::filesystem::create_directories(kPackageDir);
    }

    CURL* curl = curl_easy_init();
    if (!curl) {
        LOG(ERROR) << "Failed to initialize curl.";
        return false;
    }

    std::string response_data;
    curl_easy_setopt(curl, CURLOPT_URL, kPackageListUrl.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_data);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

    CURLcode res = curl_easy_perform(curl);

    long response_code;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
    curl_easy_cleanup(curl);

    if (res != CURLE_OK) {
        LOG(ERROR) << "Failed to fetch package list: " << curl_easy_strerror(res);
        return false;
    }

    if (response_code != 200) {
        LOG(ERROR) << "Server returned HTTP code: " << response_code;
        return false;
    }

    if (response_data.empty()) {
        LOG(ERROR) << "Package list response is empty.";
        return false;
    }

    std::ofstream out_file(kPackageListPath);
    if (!out_file) {
        LOG(ERROR) << "Failed to write package list file.";
        return false;
    }
    out_file << response_data;
    out_file.close();

    LOG(INFO) << "Package list updated successfully.";
    return true;
}

bool download_package(const std::string& package_name) {
	std::ifstream metadata_file(kPackageListPath);
    if (!metadata_file) {
        LOG(ERROR) << "Package list not found at " << kPackageListPath;
        return false;
    }

	nlohmann::json metadata;
    try {
        metadata_file >> metadata;
    } catch (const std::exception& e) {
        LOG(ERROR) << "Failed to parse JSON package list: " << e.what();
        return false;
    }

    if (!metadata.contains("packages") || !metadata["packages"].contains(package_name)) {
        LOG(ERROR) << "Package name not found in package list: " << package_name;
        return false;
    }

    if (!metadata["packages"][package_name].contains("url") || metadata["packages"][package_name]["url"].is_null()) {
        LOG(ERROR) << "No valid URL found for package: " << package_name;
        return false;
    }

    std::string package_url = metadata["packages"][package_name]["url"];
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
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
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
    std::string package_path = kPackageDir + "/" + package_name + ".tar.gz";
    std::string hash_url = kRepoUrl + "/releases/download/" + package_name + "/" + package_name + ".sha256";

    CURL* curl = curl_easy_init();
    if (!curl) {
        LOG(ERROR) << "Failed to initialize curl for hash retrieval.";
        return false;
    }

    std::string expected_hash;
    curl_easy_setopt(curl, CURLOPT_URL, hash_url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &expected_hash);
    CURLcode res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    if (res != CURLE_OK) {
        LOG(ERROR) << "Failed to fetch SHA256 hash: " << curl_easy_strerror(res);
        return false;
    }

    expected_hash.erase(std::remove(expected_hash.begin(), expected_hash.end(), '\n'), expected_hash.end());

    if (!verify_package_hash(package_path, expected_hash)) {
        LOG(ERROR) << "Hash mismatch for package: " << package_name;
        return false;
    }

    std::string extract_command = "tar -xzf " + package_path + " -C " + kPackageDir;
    if (std::system(extract_command.c_str()) != 0) {
        LOG(ERROR) << "Failed to extract package: " << package_name;
        return false;
    }

    std::filesystem::remove(package_path);

    std::string install_script = kPackageDir + "/" + package_name + "/scripts/install.sh";
    if (std::filesystem::exists(install_script)) {
        if (std::system(("bash " + install_script).c_str()) != 0) {
            LOG(ERROR) << "Installation script failed for: " << package_name;
            return false;
        }
    }

    LOG(INFO) << "Package installed successfully: " << package_name;
    return true;
}

/*
std::string fetch_package_hash(const std::string& package_name) {
    std::ifstream metadata_file(kPackageListPath);
    if (!metadata_file) {
        LOG(ERROR) << "Package list not found at: " << kPackageListPath;
        return "";
    }
    
    nlohmann::json metadata;
    try {
        metadata_file >> metadata;
    } catch (const std::exception& e) {
        LOG(ERROR) << "Failed to parse package list JSON: " << e.what();
        return "";
    }
    
    if (!metadata.contains(package_name) || !metadata[package_name].contains("sha256")) {
        LOG(ERROR) << "SHA256 hash missing in metadata for package: " << package_name;
        return "";
    }
    
    std::string hash_url = metadata[package_name]["sha256"];
    
    CURL* curl = curl_easy_init();
    if (!curl) {
        LOG(ERROR) << "Failed to initialize curl for hash retrieval.";
        return "";
    }
    
    std::ostringstream hash_stream;
    curl_easy_setopt(curl, CURLOPT_URL, hash_url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &hash_stream);
    CURLcode res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    
    if (res != CURLE_OK) {
        LOG(ERROR) << "Failed to fetch SHA256 hash: " << curl_easy_strerror(res);
        return "";
    }
    
    return hash_stream.str();
}

bool install_package(const std::string& package_name) {
    if (!download_package(package_name)) {
        return false;
    }
    
    std::string package_path = kPackageDir + "/" + package_name + ".tar.gz";
    std::string expected_hash = fetch_package_hash(package_name);
    
    if (expected_hash.empty()) {
        LOG(ERROR) << "Failed to retrieve expected hash for package: " << package_name;
        return false;
    }

    if (!verify_package_hash(package_path, expected_hash)) {
        LOG(ERROR) << "Hash mismatch for package: " << package_name;
        return false;
    }

    std::string extract_command = "tar -xzf " + package_path + " -C " + kPackageDir;
    if (std::system(extract_command.c_str()) != 0) {
        LOG(ERROR) << "Failed to extract package: " << package_name;
        return false;
    }

    std::filesystem::remove(package_path);
    
    std::string install_script = kPackageDir + "/" + package_name + "/install";
    if (std::filesystem::exists(install_script)) {
        if (std::system(install_script.c_str()) != 0) {
            LOG(ERROR) << "Installation script failed for: " << package_name;
            return false;
        }
    }

    LOG(INFO) << "Package installed successfully: " << package_name;
    return true;
}

bool self_update() {
    LOG(INFO) << "Starting self-update process...";
    return install_package(kSelfPackageName);
}

bool verify_package_hash(const std::string& package_path, const std::string& expected_hash) {
    std::ifstream file(package_path, std::ios::binary);
    if (!file.is_open()) {
        LOG(ERROR) << "Failed to open package for hash verification: " << package_path;
        return false;
    }
    
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    char buffer[8192];
    while (file.read(buffer, sizeof(buffer))) {
        SHA256_Update(&sha256, buffer, file.gcount());
    }
    file.close();

    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_Final(hash, &sha256);
    
    std::ostringstream hash_stream;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        hash_stream << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    }

    return (hash_stream.str() == expected_hash);
}*/

} 