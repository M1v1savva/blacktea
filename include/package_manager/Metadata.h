#ifndef METADATA_H
#define METADATA_H

#include <string>
#include <nlohmann/json.hpp>

struct Metadata {	
	nlohmann::json metadata;

	Metadata() {}

	inline bool load();
	bool get_url(const std::string& package_name, std::string& url);
	bool get_sha_url(const std::string& package_name, std::string& sha_url);
	bool find_name(const std::string& package_name) {}
};

#endif // METADATA_H