#include <gtest/gtest.h>
#include "package_manager.h"
#include <filesystem>
#include <fstream>

class PackageManagerTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Ensure clean test state
        std::filesystem::remove_all("packages");
    }
};

TEST_F(PackageManagerTest, FetchPackageMetadata_Success) {
    EXPECT_TRUE(package_manager::fetch_package_metadata());
    EXPECT_TRUE(std::filesystem::exists("packages/repo_list.json"));
}

TEST_F(PackageManagerTest, InstallPackage_Failure_NoMetadata) {
    EXPECT_FALSE(package_manager::install_package("nonexistent_package"));
}

TEST_F(PackageManagerTest, VerifyPackageHash_Success) {
    std::string test_file = "test_package.tar.gz";
    std::ofstream out(test_file);
    out << "dummy content";
    out.close();

    std::string expected_hash = package_manager::fetch_package_hash("test_package");
    EXPECT_FALSE(expected_hash.empty());
    EXPECT_TRUE(package_manager::verify_package_hash(test_file, expected_hash));
}

TEST_F(PackageManagerTest, SelfUpdate_Success) {
    EXPECT_TRUE(package_manager::self_update());
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
