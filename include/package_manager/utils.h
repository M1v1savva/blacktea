#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <filesystem>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <glog/logging.h>

namespace utils {

const std::string kRepoUrl = "https://github.com/M1v1savva/blacktea";
const std::string kPackageListUrl = kRepoUrl + "/releases/download/package_list_v0.1/package_list.json";
const std::string kSelfFullName = "blacktea_package_manager_v0.1";
const std::string kSelfShortName = "blacktea_package_manager";

const std::string kPackageDir = "../../packages";
const std::string kPackageListPath = kPackageDir + "/package_list.json";
const std::string kEnvListPath = kPackageDir + "/env_list.json";

size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
	size_t total_size = size * nmemb;
    output->append((char*)contents, total_size);
    return total_size;
}

void create_package_dir() {
	if (!std::filesystem::exists(kPackageDir)) {
		std::filesystem::create_directories(kPackageDir);
	}
}

struct CurledData {
	std::string data;
	CURLcode curl_response_code;
	long https_response_code; 

	CurledData(std::string& _data, CURLcode& _curl_response_code, long _https_response_code) : 
		data(_data),
		curl_response_code(_curl_response_code),
		https_response_code(_https_response_code);

	bool is_ok();
};

CurledData* fetch_data(const std::string& url);

}

#endif // UTILS_H