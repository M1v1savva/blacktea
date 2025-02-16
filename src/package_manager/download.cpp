#include "package_manager/package_manager.h"
#include "package_manager/utils.h"

bool get_package_url(const std::string& package_name, std::string& package_url) {
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

    package_url = metadata["packages"][package_name]["url"];
    return true;
}

bool package_manager::download_package(const std::string& package_name) {
	std::string package_url = "";
    get_package_url(package_name, package_url);
    
    std::string package_path = kPackageDir + "/" + package_name + ".tar.gz";

    std::ofstream out_file(package_path, std::ios::binary);
    if (!out_file.is_open()) {
        LOG(ERROR) << "Failed to create package file at: " << package_path;
        curl_easy_cleanup(curl);
        return false;
    }

    if (!utils::download(url, out_file)) {
        return false;
    }

    LOG(INFO) << "Download complete: " << package_path;
    return true;
}

}