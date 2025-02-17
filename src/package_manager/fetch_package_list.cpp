#include "package_manager/package_manager.h"
#include "package_manager/utils.h"

#include <fstream>
#include <filesystem>
#include <glog/logging.h>

namespace package_manager {

bool fetch_package_list() {
	// TODO: exception if directory cannot be created	
	if (!std::filesystem::exists(utils::config-> package_dir)) {
		std::filesystem::create_directories(utils::config-> package_dir);
	}
	
	std::ofstream out_file(kPackageListPath);
    if (!out_file) {
        LOG(ERROR) << "Failed to write package list file.";
        return false;
    }

    if (!utils::download(kPackageListUrl, out_file))
        return false;
    LOG(INFO) << "Package list updated successfully.";
    return true;
}

}