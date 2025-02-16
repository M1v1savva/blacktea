#ifndef CURLED_DATA_H
#define CURLED_DATA_H

#include <string>
#include <curl/curl.h>

class CurledData {
public:	
	std::string data;
	CURLcode curl_response_code;
	long https_response_code; 

	CurledData(std::string& _data, CURLcode& _curl_response_code, long _https_response_code)
		:
		data(_data),
		curl_response_code(_curl_response_code),
		https_response_code(_https_response_code) 
	{}
	bool download(const std::string& url, const std::ofstream& out_file);
private:
	void fetch(const std::string& url);
	bool is_ok();
};

#endif // CURLED_DATA_H