#ifndef PACKAGE_MANAGER_H
#define PACKAGE_MANAGER_H

#include <string>

namespace package_manager {

/// Fetches metadata for core packages in a remote repository.
/// @return True on success, false on failure.
bool fetch_package_metadata();

/// Downloads the specified package.
/// @param package_name Name of the package.
/// @return True on success, false on failure.
bool download_package(const std::string& package_name);

/// Installs the specified package.
/// @param package_name Name of the package.
/// @return True on success, false on failure.
bool install_package(const std::string& package_name);

/// Performs a self-update of the package manager. 
/// @return True on success, false on failure.
bool self_update();

} // namespace package_manager

#endif // PACKAGE_MANAGER_H