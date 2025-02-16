#include "package_manager/package_manager.h"
#include "package_manager/utils.h"

namespace package_manager {

bool fetch_package_list() {
	// TODO: exception if directory cannot be created	
	utils::create_package_dir();
	
	std::ofstream out_file(kPackageListPath);
    if (!out_file) {
        LOG(ERROR) << "Failed to write package list file.";
        return false;
    }

	utils::CurledData* response = utils::fetch_data(kPackageListUrl);

	if (!response-> is_ok())
		return false;

    out_file << (repsonse-> data);
    out_file.close();

    LOG(INFO) << "Package list updated successfully.";
    return true;
}

}