//============================================================================
// @name        : NetworkParserTests.cpp
// @author      : Ward Gauderis
// @date        : 3/23/19
// @version     : 1.0
// @copyright   : Project Software Engineering - BA1 Informatica - Ward Gauderis - University of Antwerp
// @description : Tests for NetworkParser.
//============================================================================


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
    Network *network = parser.parseNetwork(parser.getRoot());
    network->startSimulation(100, false);
    EXPECT_EQ(55, network->getTicksPassed());
    //TODO cerr warnings
}

TEST_F(NetworkParserTester, GetNetwork) {
    NetworkParser parser;
    EXPECT_DEATH(parser.getNetwork(), "Failed to parse network: no network");
}

TEST_F(NetworkParserTester, Init) {
    NetworkParser parser;
    EXPECT_DEATH(parser.getNetwork(), "Failed to parse network: no network");
}

TEST_F(NetworkParserTester, CompareVehiclePointers) {
    NetworkParser parser;
    EXPECT_DEATH(parser.compareVehiclePointers(NULL, NULL), "Failed to compare vehicle pointers: no vehicles");
}

