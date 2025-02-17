#include "package_manager/utils.h"

#include <fstream>
#include <nlohmann/json.hpp>
#include <glog/logging.h>

inline bool config::load(const std::string& config_file) {
	std::ifstream file(config_file);
    if (!file.is_open()) {
        LOG(ERROR) << "Failed to open config file: " << config_file;
        throw std::runtime_error("Failed to open config file: " + config_file);
    }

    nlohmann::json j;
    file >> j;

    config::repo_url = j["repo_url"];
    config::package_list_url = j["package_list_url"];
    config::self_name = j["self_name"];
    config::self_full_name = j["self_full_name"];
    config::package_dir = j["package_dir"];
    config::package_list_path = j["package_list_path"];
    config::env_list_path = j["env_list_path"];
}

config::load("../config.json");