#include <gtest/gtest.h>
#include <ctime>
int main(int argc, char*argv[])
{
    testing::InitGoogleTest(&argc, argv);
    srand(time(nullptr));
    return RUN_ALL_TESTS();
}