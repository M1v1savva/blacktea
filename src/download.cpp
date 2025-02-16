#include "package_manager/package_manager.h"
#include "package_manager/utils.h"

namespace package_manager {

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

}