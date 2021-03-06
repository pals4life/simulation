//============================================================================
// @name        : testMain.cpp
// @author      : Thomas Dooms
// @date        : 3/19/19
// @version     : 
// @copyright   : BA1 Informatica - Thomas Dooms - University of Antwerp
// @description : 
//============================================================================

#include <gtest/gtest.h>

int main(int argc, char **argv)
{
    ::testing::FLAGS_gtest_death_test_style = "threadsafe";
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

