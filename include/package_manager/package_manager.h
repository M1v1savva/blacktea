#ifndef PACKAGE_MANAGER_H
#define PACKAGE_MANAGER_H

#include <string>

namespace package_manager {

/// Fetches metadata (name, version, download URL) of the core packages. 
/// Metadata is downloaded in a json from a github release.
/// @return True on success, false on failure.
bool fetch_package_list();

/// Downloads the specified package by name.
/// Get the download [URL] in the package list.
/// @param package_name Name of the package.
/// @return True on success, false on failure.
bool download_package(const std::string& package_name);

/// Installs the specified package.
/// Before installation, fetch sha256 hash and verify the archive.
/// After that, run scripts/uninstall.sh and, subsequently, scripts/install.sh in the new manager. 
/// @param package_name Name of the package.
/// @return True on success, false on failure.
bool install_package(const std::string& package_name);

/// Performs a self-update of the package manager. 
/// Closes the package manager and runs reinstall script in the new package that overwrites the old manager.
/// @return True on success, false on failure.
bool self_update();

/// Runs package in a new process.
/// @param package_name Name of the package.
/// @return True on success, false on failure.
bool run_package(const std::string& package_name);

/// Deletes all instances of the package and archives - depending on keep_archives flag.
/// @param package_name Name of the package.
/// @param keep_archives Keep archives if true, remove otherwise.
/// @return True on success, false on failure.
bool delete_package(const std::string& package_name, const bool keep_archives=True);

} // namespace package_manager

#endif // PACKAGE_MANAGER_H