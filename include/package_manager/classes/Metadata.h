#ifndef METADATA_H
#define METADATA_H

#include <string>
#include <nlohmann/json.hpp>

struct Metadata {	
	nlohmann::json metadata;

	Metadata() {}

	inline bool load();
	bool get_url(const std::string& package_name, std::string& url);
	bool get_url_sha(const std::string& package_name, std::string& url_sha);
	bool find_name(const std::string& package_name);
};

#endif // METADATA_H