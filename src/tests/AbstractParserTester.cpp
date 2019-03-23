//============================================================================
// @name        : AbstractParserTester.cpp
// @author      : Ward Gauderis
// @date        : 3/23/19
// @version     : 1.0
// @copyright   : Project Software Engineering - BA1 Informatica - Ward Gauderis - University of Antwerp
// @description : Tests for AbstractParser.
//============================================================================

#include <gtest/gtest.h>
#include "../parsers/VAbstractParser.h"

class AbstractParserTester : public ::testing::Test {
protected:
    friend class VAbstractParser;

    virtual void SetUp() {}

    virtual void TearDown() {}

};

TEST_F(AbstractParserTester, LoadFile) {
    VAbstractParser parser;
    EXPECT_DEATH(parser.loadFile(""), "Failed to load file: no filename");
    EXPECT_DEATH(parser.loadFile("FakeFile"), "Assertion `Failed to open file' failed.");
    EXPECT_DEATH(parser.loadFile("inputfiles/testinputs/test4.xml"), "Assertion `Error reading end tag.' failed.");
    EXPECT_DEATH(parser.loadFile("inputfiles/testinputs/test5.xml"), "Assertion `Error reading end tag.' failed.");
    EXPECT_DEATH(parser.loadFile("inputfiles/testinputs/test6.xml"), "Assertion `Error reading end tag.' failed.");
    EXPECT_DEATH(parser.loadFile("inputfiles/testinputs/test7.xml"), "Assertion `Error reading end tag.' failed.");
    EXPECT_DEATH(parser.loadFile("inputfiles/testinputs/test8.xml"), "Assertion `Error reading end tag.' failed.");
    EXPECT_DEATH(parser.loadFile("inputfiles/testinputs/test9.xml"), "Assertion `Error reading end tag.' failed.");
    EXPECT_DEATH(parser.loadFile("inputfiles/testinputs/test10.xml"), "Assertion `Error reading end tag.' failed.");
}

TEST_F(AbstractParserTester, GetRoot) {
    VAbstractParser parser;
    EXPECT_DEATH(parser.getRoot(), "Failed to get root: no root element");
}

TEST_F(AbstractParserTester, ReadElement) {
    VAbstractParser parser;
    EXPECT_DEATH(parser.readElement(NULL, "FakeElement"), "Failed to read element: no element");
    parser.loadFile("inputfiles/testinputs/test1.xml");
    EXPECT_STREQ(parser.readElement(parser.getRoot(), "FakeElement").c_str(), "");
    EXPECT_DEATH(parser.readElement(parser.getRoot(), ""), "Failed to read element: no tag");
}

TEST_F(AbstractParserTester, Clear) {
    VAbstractParser parser;
    parser.loadFile("inputfiles/testinputs/test1.xml");
    parser.clear();
    EXPECT_DEATH(parser.getRoot(), "Failed to get root: no root element");
}

TEST_F(AbstractParserTester, Init) {
    VAbstractParser parser;
    EXPECT_DEATH(parser.getRoot(), "Failed to get root: no root element");
}

