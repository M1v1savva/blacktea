#include "package_manager/package_manager.h"

int main() {
	package_manager::fetch_package_list();
	//package_manager::download_package("hello_package");
	return 0;
}