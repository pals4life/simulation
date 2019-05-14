//============================================================================
// @name        : TrafficSignParserTester.cpp
// @author      : Ward Gauderis
// @date        : 5/1/19
// @version     : 2.0
// @copyright   : BA1 Informatica - Ward Gauderis - University of Antwerp
// @description : Tests for TrafficSignParser
//============================================================================

#include <gtest/gtest.h>
#include <fstream>
#include "Utils.h"
#include "../parsers/TrafficSignParser.h"

class TrafficSignParserTester : public ::testing::Test {
protected:
    friend class TrafficSignParser;

    virtual void SetUp() {}

    virtual void TearDown() {}

};

TEST_F(TrafficSignParserTester, Init) {
    TrafficSignParser parser;
    EXPECT_DEATH(parser.getZone(), "Failed to parse zone: no zone");
    EXPECT_DEATH(parser.getBusStop(), "Failed to parse bus stop: no bus stop");
    EXPECT_DEATH(parser.getTrafficLight(), "Failed to parse traffic light: no traffic light");
}

TEST_F(TrafficSignParserTester, ParseTrafficsign) {
    TrafficSignParser parser;
    testing::internal::CaptureStderr();
    bool loaded = parser.loadFile("inputfiles/testinputs/test12.xml");
    testing::internal::GetCapturedStderr();
    EXPECT_EQ(true, loaded);
    if (loaded) {
        EXPECT_DEATH(parser.parseTrafficSign(NULL), "Failed to parse traffic sign: no element");
        TiXmlElement *elem = parser.getRoot()->FirstChildElement();
        ETrafficSigns temp = parser.parseTrafficSign(elem);
        EXPECT_EQ(temp, kBusStop);
        delete parser.getBusStop();
        elem = elem->NextSiblingElement();
        testing::internal::CaptureStderr();
        do {
            EXPECT_EQ(kError, parser.parseTrafficSign(elem));
            elem = elem->NextSiblingElement();
        } while (elem != NULL);
        std::ofstream out("outputfiles/testoutputs/TrafficSignParserTester-ParseTrafficsign.txt");
        EXPECT_TRUE(out.is_open());
        out << testing::internal::GetCapturedStderr();
        out.close();
        EXPECT_TRUE(FileCompare("outputfiles/testoutputs/TrafficSignParserTester-ParseTrafficsign.txt",
                                "outputfiles/testoutputs/TrafficSignParserTester-ParseTrafficsign-expected.txt"));
    }
}

TEST_F(TrafficSignParserTester, GetTrafficLight) {
    TrafficSignParser parser;
    EXPECT_DEATH(parser.getTrafficLight(), "Failed to parse traffic light: no traffic light");
}

TEST_F(TrafficSignParserTester, getBusStop) {
    TrafficSignParser parser;
    EXPECT_DEATH(parser.getBusStop(), "Failed to parse bus stop: no bus stop");
}

TEST_F(TrafficSignParserTester, getTrafficLight) {
    TrafficSignParser parser;
    EXPECT_DEATH(parser.getZone(), "Failed to parse zone: no zone");
}

TEST_F(TrafficSignParserTester, ParseRoad) {
    TrafficSignParser parser;
    EXPECT_DEATH(parser.parseRoad(NULL), "Failed to parse road: no element");
}
