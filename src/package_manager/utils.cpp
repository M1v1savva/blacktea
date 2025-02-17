#include "package_manager/utils.h"
#include "package_manager/config.h"
#include "package_manager/Metadata.h"

#include <fstream>
#include <glog/logging.h>

bool utils::download_package(const std::string& package_name, std::ofstream& out_file) {
	Metadata* mt = new Metadata;
	std::string url;
	bool res = mt-> get_url(package_name, url);
	if (!res) return false;
	return new CurledData()-> download(url, out_file);
}

bool utils::download_sha256(const std::string& url_sha, std::string& out_data) {
	Metadata* mt = new Metadata;
	std::string url;
	bool res = mt-> get_url_sha(package_name, url_sha);
	if (!res) return false;
	CurledData* hs = new CurledData();
	if (!hs-> fetch(url_sha))
		return false;
	out_data = hs-> data;
	return true;
}


