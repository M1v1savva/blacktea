#include "package_manager/CurledData.h"

#include <fstream>
#include <glog/logging.h>

bool CurledData::download(const std::string& url, const std::ofstream& out_file) {
    this-> fetch(url);
    if (!this-> is_ok()) {
        return false;
    }
    
    out_file << (response-> data);
    out_file.close();
    return true;
}

void CurledData::fetch(const std::string& url) {
	CURL* curl = curl_easy_init();
    if (!curl) {
        LOG(ERROR) << "Failed to initialize curl.";
        return false;
    }

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &(this-> data));
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

    this-> curl_response_code = curl_easy_perform(curl);

    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &(this-> https_response_code));
    curl_easy_cleanup(curl);
}

bool CurledData::is_ok() {
	if (this-> curl_response_code != CURLE_OK) {
        LOG(ERROR) << "Failed to fetch package list: " << curl_easy_strerror(res);
        return false;
    }

    if (this-> https_response_code != 200) {
        LOG(ERROR) << "Server returned HTTPS code: " << response_code;
        return false;
    }

    if ((this-> data).empty()) {
        LOG(ERROR) << "Package list response is empty.";
        return false;
    }
	return true;
}