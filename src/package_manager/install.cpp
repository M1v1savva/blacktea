#include "package_manager/package_manager.h"

bool package_manager::install_package(const std::string& package_name) {
	std::string package_url = "";
    get_package_url(package_name, package_url);
    
    std::string package_path = util::config-> package_dir + "/" + util::config-> package_full_name + ".tar.gz";

    std::ofstream out_file(package_path, std::ios::binary);
    if (!out_file.is_open()) {
        LOG(ERROR) << "Failed to create package file at: " << package_path;
        curl_easy_cleanup(curl);
        return false;
    }

    if (!utils::download(url, out_file)) {
        return false;
    }

	

	return true;
}