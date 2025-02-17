#include "package_manager/classes/Metadata.h"
#include "package_manager/utils.h"

#include <fstream>
#include <glog/logging.h>

inline bool Metadata::load() {
	std::ifstream metadata_file(config::package_list_path);
    if (!metadata_file) {
        LOG(ERROR) << "Package list not found at " << config::package_list_path;
        return false;
    }
	try {
        metadata_file >> (this-> metadata);
    } catch (const std::exception& e) {
        LOG(ERROR) << "Failed to parse JSON package list: " << e.what();
        return false;
    }
}

bool Metadata::get_url(const std::string& package_name, std::string& url) {
	if (!(this-> find_name(package_name)))
		return false;

    if (!metadata["packages"][package_name].contains("url") || metadata["packages"][package_name]["url"].is_null()) {
        LOG(ERROR) << "No valid URL found for package: " << package_name;
        return false;
    }

    url = metadata["packages"][package_name]["url"];
    return true;
}

bool Metadata::get_url_sha(const std::string& package_name, std::string& url_sha) {
	if (!(this-> find_name(package_name)))
		return false;

    if (!metadata["packages"][package_name].contains("url_sha") || metadata["packages"][package_name]["url_sha"].is_null()) {
        LOG(ERROR) << "No valid SHA256 URL found for package: " << package_name;
        return false;
    }

    url_sha = metadata["packages"][package_name]["url_sha"];
    return true;
}

bool Metadata::find_name(const std::string& package_name) {
	if (!(this-> metadata).contains("packages") || !(this-> metadata)["packages"].contains(package_name)) {
        LOG(ERROR) << "Package name not found in package list: " << package_name;
        return false;
    }
    return true;
}