#include "package_manager/utils.h"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <curl/curl.h>
#include <glog/logging.h>
#include <nlohmann/json.hpp>
#include <cstdlib>

namespace utils {

bool CurledData::is_ok() {
	if (this-> curl_response_code != CURLE_OK) {
        LOG(ERROR) << "Failed to fetch package list: " << curl_easy_strerror(res);
        return false;
    }

    if (this-> https_response_code != 200) {
        LOG(ERROR) << "Server returned HTTPS code: " << response_code;
        return false;
    }

    if (this-> data.empty()) {
        LOG(ERROR) << "Package list response is empty.";
        return false;
    }
	return true;
}

CurledData* fetch_data() {
	CURL* curl = curl_easy_init();
    if (!curl) {
        LOG(ERROR) << "Failed to initialize curl.";
        return false;
    }

    std::string response_data;
    curl_easy_setopt(curl, CURLOPT_URL, kPackageListUrl.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_data);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

    CURLcode curl_response_code = curl_easy_perform(curl);

    long https_response_code;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &https_response_code);
    curl_easy_cleanup(curl);

	CurledData* response = new CurledData(response_data, curl_response_code, https_response_code); 
	return response;
}

}