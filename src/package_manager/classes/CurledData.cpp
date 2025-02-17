#include "package_manager/classes/CurledData.h"
#include "package_manager/utils.h"

#include <fstream>
#include <curl/curl.h>
#include <glog/logging.h>

bool CurledData::download(const std::string& url, std::ofstream& out_file) {
    if (!this-> fetch(url))
        return false;
    if (!this-> is_ok()) {
        return false;
    }
    
    out_file << (this-> data);
    out_file.close();
    return true;
}

bool CurledData::fetch(const std::string& url) {
	CURL* curl = curl_easy_init();
    if (!curl) {
        LOG(ERROR) << "Failed to initialize curl.";
        return false;
    }

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, utils::WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &(this-> data));
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

    this-> curl_response_code = curl_easy_perform(curl);

    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &(this-> https_response_code));
    curl_easy_cleanup(curl);

    return true;
}

bool CurledData::is_ok() {
	if (this-> curl_response_code != CURLE_OK) {
        LOG(ERROR) << "Failed to fetch package list: " << curl_easy_strerror(this-> curl_response_code);
        return false;
    }

    if (this-> https_response_code != 200) {
        LOG(ERROR) << "Server returned HTTPS code: " << this-> https_response_code;
        return false;
    }

    if ((this-> data).empty()) {
        LOG(ERROR) << "Package list response is empty.";
        return false;
    }
	return true;
}