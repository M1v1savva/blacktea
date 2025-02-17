#include "package_manager/package_manager.h"
#include "package_manager/utils.h"
#include "package_manager/CurledData.h"
#include "package_manager/Metadata.h"

#include <filesystem>
#include <fstream>
#include <glog/logging.h>
#include <gtest/gtest.h>

class PackageManagerTest : public ::testing::Test {
protected:
    void SetUp() override {
        std::filesystem::remove_all("packages");
    }
};

TEST_F(PackageManagerTest, LoadConfig_Success) {
    EXPECT_TRUE((utils::config-> self_name) == "blacktea_package_manager");
    LOG(INFO) << utils::config-> self_name;
    LOG(INFO) << utils::config-> package_list_url;
}

// TEST_F(PackageManagerTest, FetchPackageMetadata_Success) {
//     EXPECT_TRUE(package_manager::fetch_package_list());
//     EXPECT_TRUE(std::filesystem::exists("../../packages/package_list.json"));
// }

// TEST_F(PackageManagerTest, DownloadPackage_Success) {
//     EXPECT_TRUE(package_manager::download_package("hello_package"));
// }

// TEST_F(PackageManagerTest, DownloadPackage_Failure_NoMetadata) {
//     EXPECT_FALSE(package_manager::download_package("nonexistent_package"));
// }

// TEST_F(PackageManagerTest, VerifyPackageHash_Success) {
//     std::string test_file = "test_package.tar.gz";
//     std::ofstream out(test_file);
//     out << "dummy content";
//     out.close();

//     std::string expected_hash = package_manager::fetch_package_hash("test_package");
//     EXPECT_FALSE(expected_hash.empty());
//     EXPECT_TRUE(package_manager::verify_package_hash(test_file, expected_hash));
// }

// TEST_F(PackageManagerTest, SelfUpdate_Success) {
//     EXPECT_TRUE(package_manager::self_update());
// }

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
