//============================================================================
// @name        : GuiTester.cpp
// @author      : Mano Marichal
// @date        : 19.03.19
// @version     : 1.0
// @copyright   : BA1 Informatica - Thomas Dooms - University of Antwerp
// @description :
//============================================================================

#include <gtest/gtest.h>
#include "../gui/gui.h"

class GuiTester : public ::testing::Test
{
protected:
    virtual void SetUp(){}
    virtual void TearDown(){}
};

TEST_F(GuiTester, rounding1)
{
    EXPECT_EQ("1,011", Window::doubleToPrecision(1.0111111, 3));
}

TEST_F(GuiTester, rounding2)
{
    EXPECT_EQ("1,012", Window::doubleToPrecision(1.0116241341, 3));
}

TEST_F(GuiTester, rounding3)
{
    EXPECT_EQ("-1,010", Window::doubleToPrecision(-1.01111111321, 3));
}

TEST_F(GuiTester, rounding4)
{
    EXPECT_EQ("-1,011", Window::doubleToPrecision(-1.01174325432, 3));
}

TEST_F(GuiTester, rounding5)
{
    EXPECT_EQ("1,", Window::doubleToPrecision(1.011447341, 0));
}

TEST_F(GuiTester, rounding6)
{
    EXPECT_EQ("2,", Window::doubleToPrecision(1.911447341, 0));
}

TEST_F(GuiTester, rounding7)
{
    EXPECT_EQ("1,1111", Window::doubleToPrecision(1.1111, 4));
}

TEST_F(GuiTester, rounding8)
{
    EXPECT_EQ("1,1000", Window::doubleToPrecision(1.1, 4));
}