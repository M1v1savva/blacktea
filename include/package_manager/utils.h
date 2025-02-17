#ifndef UTILS_H
#define UTILS_H

#include <string>

namespace utils {

inline size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
	size_t total_size = size * nmemb;
    output->append((char*)contents, total_size);
    return total_size;
}

bool download_package(const std::string& package_name);
bool compare_sha256(const std::string& url_sha, const std::string& archive_path);

}

#endif // UTILS_H