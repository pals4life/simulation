//
// Created by ward on 3/23/19.
//
//TODO header

#include <gtest/gtest.h>
#include "../parsers/NetworkParser.h"

class NetworkParserTester : public ::testing::Test {
protected:
    friend class NetworkParser;
    virtual void SetUp() {}

    virtual void TearDown() {}

};

TEST_F(NetworkParserTester, ParseNetwork) {
    NetworkParser parser;
    parser.loadFile("inputfiles/testinputs/test1.xml");
    EXPECT_DEATH(parser.parseNetwork(NULL), "Failed to parse network: no element");
}

