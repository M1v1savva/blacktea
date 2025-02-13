#include <iostream>
#include "hello_package/greeting.h"

int main() {
    std::cout << hello_package::get_greeting() << std::endl;
    return 0;
}