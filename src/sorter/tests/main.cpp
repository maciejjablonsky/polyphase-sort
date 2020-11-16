#include <gtest/gtest.h>
#include "TestConfig.hpp"

int main(const int argc, const char*argv[])
{
    testing::InitGoogleTest(const_cast<int*>(&argc), const_cast<char**>(argv));
    srand(time(nullptr));
    TestConfig::Config(argc, argv);
    return RUN_ALL_TESTS();
}