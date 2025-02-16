#ifndef UTILS_H
#define UTILS_H

#include <string>

namespace utils {

struct Config {
    std::string repo_url,
                package_list_url,
                self_name,
                self_full_name,
                package_dir,
                package_list_path,
                env_list_path;

    bool load_config();
};

size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
	size_t total_size = size * nmemb;
    output->append((char*)contents, total_size);
    return total_size;
}

}

#endif // UTILS_H