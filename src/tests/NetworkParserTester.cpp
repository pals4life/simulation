//============================================================================
// @name        : NetworkParserTests.cpp
// @author      : Ward Gauderis
// @date        : 3/23/19
// @version     : 1.0
// @copyright   : Project Software Engineering - BA1 Informatica - Ward Gauderis - University of Antwerp
// @description : Tests for NetworkParser.
//============================================================================

//TODO no interrupts

#include <gtest/gtest.h>
#include "../parsers/NetworkParser.h"
#include "Utils.h"

class NetworkParserTester : public ::testing::Test {
protected:
    friend class NetworkParser;

    virtual void SetUp() {}

    virtual void TearDown() {}

};

TEST_F(NetworkParserTester, ParseNetwork) {
    NetworkParser parser;
    testing::internal::CaptureStderr();
    bool loaded = parser.loadFile("inputfiles/testinputs/test1.xml");
    testing::internal::GetCapturedStderr();
    EXPECT_EQ(true, loaded);
    if (loaded) {
        EXPECT_DEATH(parser.parseNetwork(NULL), "Failed to parse network: no element");
        Network *network = parser.parseNetwork(parser.getRoot());
        EXPECT_TRUE(network);
        testing::internal::CaptureStdout();
        network->startSimulation(NULL, "simple", "impression", true);
        testing::internal::GetCapturedStdout();
        EXPECT_EQ(230, network->getTicksPassed());
        delete network;
    }
    testing::internal::CaptureStderr();
    loaded = parser.loadFile("inputfiles/testinputs/test11.xml");
    testing::internal::GetCapturedStderr();
    EXPECT_EQ(true, loaded);
    if (loaded) {
        testing::internal::CaptureStderr();
        Network *temp = parser.parseNetwork(parser.getRoot());
        delete temp;
        std::ofstream out("outputfiles/testoutputs/NetworkParserTester-ParseNetwork.txt");
        EXPECT_TRUE(out.is_open());
        out << testing::internal::GetCapturedStderr();
        out.close();
        EXPECT_TRUE(FileCompare("outputfiles/testoutputs/NetworkParserTester-ParseNetwork.txt",
                                "outputfiles/testoutputs/NetworkParserTester-ParseNetwork-expected.txt"));
    }
}

TEST_F(NetworkParserTester, GetNetwork) {
    NetworkParser parser;
    EXPECT_DEATH(parser.getNetwork(), "Failed to parse network: no network");
}

TEST_F(NetworkParserTester, Init) {
    NetworkParser parser;
    EXPECT_DEATH(parser.getNetwork(), "Failed to parse network: no network");
}
