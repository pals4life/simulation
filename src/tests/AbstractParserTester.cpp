//============================================================================
// @name        : AbstractParserTester.cpp
// @author      : Ward Gauderis
// @date        : 3/23/19
// @version     : 1.0
// @copyright   : Project Software Engineering - BA1 Informatica - Ward Gauderis - University of Antwerp
// @description : Tests for AbstractParser.
//============================================================================

#include <gtest/gtest.h>
#include "Utils.h"
#include <fstream>
#include "../parsers/VAbstractParser.h"

class AbstractParserTester : public ::testing::Test {
protected:
    friend class VAbstractParser;

    virtual void SetUp() {}

    virtual void TearDown() {}

};

TEST_F(AbstractParserTester, LoadFile) {
    VAbstractParser parser;
    testing::internal::CaptureStderr();
    EXPECT_FALSE(parser.loadFile(""));
    EXPECT_FALSE(parser.loadFile("FakeFile"));
    EXPECT_FALSE(parser.loadFile("inputfiles/testinputs/test4.xml"));
    EXPECT_FALSE(parser.loadFile("inputfiles/testinputs/test5.xml"));
    EXPECT_FALSE(parser.loadFile("inputfiles/testinputs/test6.xml"));
    EXPECT_FALSE(parser.loadFile("inputfiles/testinputs/test7.xml"));
    EXPECT_FALSE(parser.loadFile("inputfiles/testinputs/test8.xml"));
    EXPECT_FALSE(parser.loadFile("inputfiles/testinputs/test9.xml"));
    EXPECT_FALSE(parser.loadFile("inputfiles/testinputs/test10.xml"));
    std::ofstream out("outputfiles/testoutputs/AbstractParserTester-LoadFile.txt");
    EXPECT_TRUE(out.is_open());
    out << testing::internal::GetCapturedStderr();
    out.close();
    EXPECT_TRUE(FileCompare("outputfiles/testoutputs/AbstractParserTester-LoadFile.txt",
                            "outputfiles/testoutputs/AbstractParserTester-LoadFile-expected.txt"));
}

TEST_F(AbstractParserTester, GetRoot) {
    VAbstractParser parser;
    EXPECT_DEATH(parser.getRoot(), "Failed to get root: no root element");
}

TEST_F(AbstractParserTester, ReadElement) {
    VAbstractParser parser;
    EXPECT_DEATH(parser.readElement(NULL, "FakeElement"), "Failed to read element: no element");
    testing::internal::CaptureStderr();
    bool loaded = parser.loadFile("inputfiles/testinputs/test1.xml");
    EXPECT_EQ(true, loaded);
    testing::internal::GetCapturedStderr();
    if (loaded) {
        EXPECT_STREQ(parser.readElement(parser.getRoot(), "FakeElement").c_str(), "");
        EXPECT_DEATH(parser.readElement(parser.getRoot(), ""), "Failed to read element: no tag");
    }
}

TEST_F(AbstractParserTester, Clear) {
    VAbstractParser parser;
    testing::internal::CaptureStderr();
    bool loaded = parser.loadFile("inputfiles/testinputs/test1.xml");
    EXPECT_EQ(true, loaded);
    testing::internal::GetCapturedStderr();
    parser.clear();
    EXPECT_DEATH(parser.getRoot(), "Failed to get root: no root element");
}

TEST_F(AbstractParserTester, Init) {
    VAbstractParser parser;
    EXPECT_DEATH(parser.getRoot(), "Failed to get root: no root element");
}

