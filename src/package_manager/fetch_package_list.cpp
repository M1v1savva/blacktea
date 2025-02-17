#include "package_manager/package_manager.h"
#include "package_manager/utils.h"

#include <fstream>
#include <filesystem>
#include <glog/logging.h>

namespace package_manager {

bool fetch_package_list() {
	if (!std::filesystem::exists(utils::config-> package_dir) &&
		!std::filesystem::create_directories(utils::config-> package_dir)) {
		throw std::runtime_error("Failed to create directory");
	}
	
	std::ofstream out_file(utils::config-> package_list_path);
    if (!out_file) {
        LOG(ERROR) << "Failed to write package list file.";
        return false;
    }

    if (!utils::download(utils::config-> package_list_url, out_file))
        return false;
    LOG(INFO) << "Package list updated successfully.";
    return true;
}

}