#include <gtest/gtest.h>
#include "hello_package/greeting.h"

TEST(GreetingTest, ReturnsHelloWorld) {
    EXPECT_EQ(hello_package::get_greeting(), "Hello, world!");
}