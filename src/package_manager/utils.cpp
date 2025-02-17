#include "package_manager/utils.h"
#include "package_manager/config.h"
#include "package_manager/Metadata.h"

#include <fstream>
#include <glog/logging.h>

bool utils::download_package(const std::string& package_name, std::ofstream& out_file) {
	return new CurledData()-> download(package_name, out_file);
}

