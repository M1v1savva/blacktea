#ifndef PACKAGE_MANAGER_H
#define PACKAGE_MANAGER_H

#include <string>

namespace package_manager {

/// Fetches metadata (name, version, download URL) of the core packages. 
/// Metadata is downloaded in an archived json from a github release.
/// @return Fetched metadata as a json.
void* fetch_package_metadata();

/// Downloads the specified package by name.
/// Removes all archives starting with {package_name}.
/// Downloads the package from [URL] that we get with the metadata fetch.
/// @param package_name Name of the package.
/// @return True on success, false on failure.
bool download_package(const std::string& package_name);

/// Installs the specified package.
/// Before installation, ensure archive integrity.
/// After that, run scripts/install.sh script if present, otherwise scripts/build.sh. 
/// @param package_name Name of the package.
/// @return True on success, false on failure.
bool install_package(const std::string& package_name);

/// Performs a self-update of the package manager. 
/// Closes the package manager and runs install in the new package that overwrites the old manager.
/// @return True on success, false on failure.
bool self_update();

/// Downloads the package hash by name. Hashes for archives and binaries.
/// Downloads the package from [URL] that we get with the metadata fetch.
/// @param package_name Name of the package.
/// @return Fetched hash of the archives and binaries.
void* fetch_package_hash(const std::string& package_name);

/// Runs package in a new process.
/// Fetches and compares the hashes first to ensure integrity of the binaries.
/// @param package_name Name of the package.
/// @return True on success, false on failure.
bool run_package(const std::string& package_name);

/// Deletes all instances of the package and archives - depending on keep_archives.
/// @param package_name Name of the package.
/// @param keep_archives Keep archives if true, remove otherwise.
/// @return True on success, false on failure.
bool delete_package(const std::string& package_name, const bool keep_archives=True);

} // namespace package_manager

#endif // PACKAGE_MANAGER_H