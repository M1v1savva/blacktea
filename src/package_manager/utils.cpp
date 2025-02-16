#include "package_manager/utils.h"

#include <fstream>
#include <nlohmann/json.hpp>
#include <glog/logging.h>

namespace utils {

Config* config = new Config("../config.json");

Config::Config(const std::string& config_file) {
    std::ifstream file(config_file);
    if (!file.is_open()) {
        LOG(ERROR) << "Failed to open config file: " << config_file;
        throw std::runtime_error("Failed to open config file: " + config_file);
    }

    nlohmann::json j;
    file >> j;

    this-> repo_url = j["repo_url"];
    this-> package_list_url = j["package_list_url"];
    this-> self_name = j["self_name"];
    this-> self_full_name = j["self_full_name"];
    this-> package_dir = j["package_dir"];
    this-> package_list_path = j["package_list_path"];
    this-> env_list_path = j["env_list_path"];
}

}